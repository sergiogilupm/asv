

/** The continuation used in UDP server. */
class UDPServerSessionContinuation : public BSocketContinuation {
 public:
  enum {
    UDP_SERVER_SESSION_UNITIALIZED = 0,
    UDP_SERVER_SESSION_RECEIVING   = 1,
    UDP_SERVER_SESSION_CONNECTING  = 2,
    UDP_SERVER_SESSION_SENDING     = 3,
    UDP_SERVER_SESSION_CLOSING     = 4
  };
  
  UDPServerSessionContinuation(UDPServerSession* server, int ssock) : 
    BSocketContinuation(server), server_socket(ssock), 
    status(UDP_SERVER_SESSION_UNITIALIZED) {
    reqbuf = new byte[server->request_size];
    assert(reqbuf);
  }

  virtual ~UDPServerSessionContinuation() {
    delete[] reqbuf;
  }

  /* When a continuation returns successfully, this function will be
     called. Based on the current status of this continuation, we will
     decide what to do next. */
  virtual void success() {
    UDPServerSession* server = (UDPServerSession*)owner;
    switch(status) {
    case UDP_SERVER_SESSION_RECEIVING:
      server->request_handler(this);
      //server->request_received(this);
      break;
    case UDP_SERVER_SESSION_CONNECTING:
      server->client_connected(this);
      break;
    case UDP_SERVER_SESSION_SENDING:
      server->data_sent(this);
      break;
    case UDP_SERVER_SESSION_CLOSING:
      server->session_closed(this);
      delete this; // end of the journey!!!
      break;
    default:
      assert(0);
      break;
    }
  }

  /* When a continuation returns unsuccessfully, this function will be
     called to print out the error. */
  virtual void failure() {
    UDPServerSession* server = (UDPServerSession*)owner;
    switch(status) {
    case UDP_SERVER_SESSION_RECEIVING:
      UDP_DUMP(printf("UDPServerSessionContinuation::failure() on host \"%s\": "
		      "failed to receive request from client.\n",
		      server->inHost()->nhi.toString()));
      server->handle_client(server_socket);
      break;
    case UDP_SERVER_SESSION_CONNECTING:
      UDP_DUMP(printf("UDPServerSessionContinuation::failure() on host \"%s\": "
		      "failed to connect to client.\n",
		      server->inHost()->nhi.toString()));
      // since this session has failed, we should decrease the number
      // of clients in the system.
      if(server->client_limit && server->nclients-- == server->client_limit)
    	server->handle_client(server_socket);
      break;
    case UDP_SERVER_SESSION_SENDING:
      UDP_DUMP(printf("UDPServerSessionContinuation::failure() on host \"%s\": "
		      "failed to send to client.\n",
		      server->inHost()->nhi.toString()));
      // since this session has failed, we should decrease the number
      // of clients in the system.
      if(server->client_limit && server->nclients-- == server->client_limit)
    	server->handle_client(server_socket);
      break;
    case UDP_SERVER_SESSION_CLOSING:
      UDP_DUMP(printf("UDPServerSessionContinuation::failure() on host \"%s\": "
		      "failed to close the connection.\n",
		      server->inHost()->nhi.toString()));
      // since this session has failed, we should decrease the number
      // of clients in the system.
      if(server->client_limit && server->nclients-- == server->client_limit)
    	server->handle_client(server_socket);
      break;
    default:
      assert(0);
      break;
    }

    // delete this continuation.
    delete this;
  }

  void send_timeout()
  {
    ((UDPServerSession*)owner)->interval_elapsed(this);
  }

  void send_time_callback(Activation ac)
  {
	UDPServerSessionContinuation* cnt = (UDPServerSessionContinuation*)((UDPServerSessionCallbackActivation*)ac)->cnt;
	cnt->send_timeout();
  }

 public:
  int server_socket; ///< server socket session on well-known port
  int client_socket; ///< the socket connection with a client
  int status; ///< the status of this socket session
  byte* reqbuf; ///< buffer for storing request from client

  IPADDR client_ip; ///< client ip address of this socket session
  uint16 client_port; ///< client port number of this socket session
  int file_size; ///< number of bytes to be sent to the client
  int sent_bytes; ///< number of bytes already sent

  HandleCode send_timer; ///< user timer for off time
  Process* send_time_callback_proc;
  UDPServerSessionCallbackActivation* send_time_ac;
};



/** ASV IMPLEMENTATION FUNCTIONS **/

void init_array(int size)
{
	reservoir = (UDPServerSessionContinuation*) malloc (size * sizeof(UDPServerSessionContinuation));
}


void add_to_array(UDPServerSessionContinuation* const cnt)
{

}


void replace_in_array(UDPServerSessionContinuation* const cnt,int pos)
{

}


void empty_array()
{

}
