/**
 * \file udp_attacker.h
 * \brief Header file for the UDPClientSession class.
 *
 * authors : Dong (Kevin) Jin
 */

#ifndef __UDP_CLIENT_H__
#define __UDP_CLIENT_H__

#include "os/base/protocol_session.h"
#include "net/ip_prefix.h"
#include "util/shstl.h"

namespace s3f {
namespace s3fnet {

class UDPAttackerSessionContinuation;
class UDPAttackerSessionCallbackActivation;
class SocketMaster;

/**
 * \brief The UDP Attacker test protocol session.
 *
 * This is a test protocol that does not exist in reality. In a normal
 * situation, the protocol waits for a period of time before it
 * randomly selects a UDP server and tries to retrieve a number of
 * bytes from the server. The process repeats until simulation
 * ends. The difference between this class and the
 * BlockingUDPAttackerSession class is that the latter uses blocking
 * socket calls. In SSF, blocking is implemented using non-simple
 * processes that come with a detectable higher execution cost than
 * simple processes.
 */
class UDPAttackerSession: public ProtocolSession {
  friend class UDPAttackerSessionContinuation;

 public:
  /** The constructor. */
  UDPAttackerSession(ProtocolGraph* graph);

  /** The destructor. */
  virtual ~UDPAttackerSession();

  /** Configure the UDP Attacker test protocol session. */
  virtual void config(s3f::dml::Configuration *cfg);

  /**
   * Return the protocol number. This specifies the relationship
   * between this protocol and the other protocols defined in the same
   * protocol stack.
   */
  virtual int getProtocolNumber() { return S3FNET_PROTOCOL_TYPE_UDPTEST_CLIENT; } // Not changed

  /** Initialize this protocol session. */
  virtual void init();

  /** Provide call_back functionality for implementing the start timer in the UDPAttackerSession
   *  this S3F process is used by waitFor() function */
  Process* start_timer_callback_proc;
  /** The callback function registered with the start_timer_callback_proc */
  void start_timer_callback(Activation ac);
  ProtocolCallbackActivation* start_timer_ac;
  /** Storing the data for the callback function. In this case, the UDPAttackerSession object is stored. */
  bool start_timer_called_once;


  void end_of_request_wave(Activation ac);

  void end_of_attack(Activation ac);

 protected:
  // functions representing different stages
  /** the first function to call for starting a communication at Attacker side */
  void main_proc(int sample_off_time, ltime_t lead_time);
  /** first time to choose the fixed UDP server if the DML specifies to use fixed server */
  void start_once();
  /** start the socket communication, such as bind and connect, and choose random server if the DML specifies to use random server  */
  void start_on();
  /** function representing the server_connected stage */
  void server_connected(UDPAttackerSessionContinuation* const cnt);
  /** function representing the request_sent stage */
  void request_sent(UDPAttackerSessionContinuation* const cnt);
  /** function representing the data_received stage */
  void data_received(UDPAttackerSessionContinuation* const cnt);
  /** function representing the session_closed stage */
  void session_closed(UDPAttackerSessionContinuation* const cnt);

  void decrease_aux(); //ASV
  /** Get a server ip and port randomly from traffic description and
      return false if failed to do so. */

  void restart_timer();
 
  bool get_random_server(IPADDR& server_ip, uint16& server_port);

  /** Called if the file transfer takes too long. */
  void timeout(int sock);

  /** This method should not be called; it is provided here to prompt
      an error message if it's called accidentally. */
  virtual int push(Activation msg, ProtocolSession* hi_sess, void* extinfo = 0, size_t extinfo_size = 0);

  /** This method should not be called; it is provided here to prompt
      an error message if it's called accidentally. */
  virtual int pop(Activation msg, ProtocolSession* lo_sess, void* extinfo = 0, size_t extinfo_size = 0);

 private:
  // configurable parameters
  ltime_t start_time;   ///< Time before a session starts.
  double start_time_double;
  ltime_t start_window; ///< Size of random window before session starts.
  double start_window_double;
  ltime_t user_timeout;  ///< timeout before aborting a session
  ltime_t attack_time;
  ltime_t off_time; ///< Off time between sessions.
  double off_time_double;
  bool off_time_run_first; ///< Whether 1st session starts with an off time.
  bool off_time_exponential; ///< Constant or exponential off time.
  bool fixed_server; ///< Whether to find a random target.
  uint32 request_size; ///< Size of the request sent to the server.
  uint32 file_size; ///< Size of the file to be sent from the server.
  uint16 start_port; ///< The starting port number for Attacker sessions.
  S3FNET_STRING server_list; //< Traffic server list name (default: forUDP)
  bool show_report; ///< Whether we print out the result or not.


  // ASV
  int totalREQ;
  int aux;
  int received;
  int j;
  int J_MAX;

  //int REQ_max;

  int attack_rate;


  // state variables
  SocketMaster* sm; ///< Points to the socket master.
  //UDPClientSessionStartTimer* start_timer; ///< The timer used to start a client session.
  IPADDR attacker_ip; ///< The ip address of the client (interface 0).
  uint32 nsess; ///< Number of sessions initiated by this client (statistics).

  IPADDR server_ip; ///< Server ip address of the current connection.
  uint16 server_port; ///< Server port number of the current connection.
};

/**
 *  Storing the data for the callback function
 *  this case, the UDPClientSession object and the UDPClientSessionContinuation object are stored.
 */
class UDPAttackerSessionCallbackActivation : public ProtocolCallbackActivation
{
  public:
	UDPAttackerSessionCallbackActivation(ProtocolSession* sess, UDPAttackerSessionContinuation* udp_cnt) :
		ProtocolCallbackActivation(sess), cnt(udp_cnt){};
	virtual ~UDPAttackerSessionCallbackActivation(){};
	UDPAttackerSessionContinuation* cnt;
};

}; // namespace s3fnet
}; // namespace s3f

#endif /*__UDP_CLIENT_H__*/
