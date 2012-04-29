/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.33
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package net.sourceforge.playerstage.Jplayercore;

public class Driver {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Driver(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Driver obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      playercore_javaJNI.delete_Driver(swigCPtr);
    }
    swigCPtr = 0;
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    playercore_javaJNI.Driver_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    playercore_javaJNI.Driver_change_ownership(this, swigCPtr, true);
  }

  protected void StartThread() {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_StartThread(swigCPtr, this); else playercore_javaJNI.Driver_StartThreadSwigExplicitDriver(swigCPtr, this);
  }

  protected void StopThread() {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_StopThread(swigCPtr, this); else playercore_javaJNI.Driver_StopThreadSwigExplicitDriver(swigCPtr, this);
  }

  public void setDriverthread(SWIGTYPE_p_pthread_t value) {
    playercore_javaJNI.Driver_driverthread_set(swigCPtr, this, SWIGTYPE_p_pthread_t.getCPtr(value));
  }

  public SWIGTYPE_p_pthread_t getDriverthread() {
    return new SWIGTYPE_p_pthread_t(playercore_javaJNI.Driver_driverthread_get(swigCPtr, this), true);
  }

  public void Lock() {
    playercore_javaJNI.Driver_Lock(swigCPtr, this);
  }

  public void Unlock() {
    playercore_javaJNI.Driver_Unlock(swigCPtr, this);
  }

  public void setRet_queue(QueuePointer value) {
    playercore_javaJNI.Driver_ret_queue_set(swigCPtr, this, QueuePointer.getCPtr(value), value);
  }

  public QueuePointer getRet_queue() {
    long cPtr = playercore_javaJNI.Driver_ret_queue_get(swigCPtr, this);
    return (cPtr == 0) ? null : new QueuePointer(cPtr, false);
  }

  public void Publish(player_devaddr_t addr, QueuePointer queue, short type, short subtype, SWIGTYPE_p_void src, long deprecated, SWIGTYPE_p_double timestamp, boolean copy) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_0(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp), copy); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_0(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp), copy);
  }

  public void Publish(player_devaddr_t addr, QueuePointer queue, short type, short subtype, SWIGTYPE_p_void src, long deprecated, SWIGTYPE_p_double timestamp) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_1(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_1(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp));
  }

  public void Publish(player_devaddr_t addr, QueuePointer queue, short type, short subtype, SWIGTYPE_p_void src, long deprecated) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_2(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_2(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated);
  }

  public void Publish(player_devaddr_t addr, QueuePointer queue, short type, short subtype, SWIGTYPE_p_void src) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_3(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_3(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype, SWIGTYPE_p_void.getCPtr(src));
  }

  public void Publish(player_devaddr_t addr, QueuePointer queue, short type, short subtype) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_4(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_4(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, QueuePointer.getCPtr(queue), queue, type, subtype);
  }

  public void Publish(player_devaddr_t addr, short type, short subtype, SWIGTYPE_p_void src, long deprecated, SWIGTYPE_p_double timestamp, boolean copy) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_5(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp), copy); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_5(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp), copy);
  }

  public void Publish(player_devaddr_t addr, short type, short subtype, SWIGTYPE_p_void src, long deprecated, SWIGTYPE_p_double timestamp) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_6(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_6(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated, SWIGTYPE_p_double.getCPtr(timestamp));
  }

  public void Publish(player_devaddr_t addr, short type, short subtype, SWIGTYPE_p_void src, long deprecated) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_7(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_7(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src), deprecated);
  }

  public void Publish(player_devaddr_t addr, short type, short subtype, SWIGTYPE_p_void src) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_8(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_8(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype, SWIGTYPE_p_void.getCPtr(src));
  }

  public void Publish(player_devaddr_t addr, short type, short subtype) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_9(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_9(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr, type, subtype);
  }

  public void Publish(QueuePointer queue, player_msghdr_t hdr, SWIGTYPE_p_void src, boolean copy) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_10(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src), copy); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_10(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src), copy);
  }

  public void Publish(QueuePointer queue, player_msghdr_t hdr, SWIGTYPE_p_void src) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_11(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_11(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src));
  }

  public void Publish(player_msghdr_t hdr, SWIGTYPE_p_void src, boolean copy) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_12(swigCPtr, this, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src), copy); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_12(swigCPtr, this, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src), copy);
  }

  public void Publish(player_msghdr_t hdr, SWIGTYPE_p_void src) {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Publish__SWIG_13(swigCPtr, this, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src)); else playercore_javaJNI.Driver_PublishSwigExplicitDriver__SWIG_13(swigCPtr, this, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(src));
  }

  public void setDevice_addr(player_devaddr_t value) {
    playercore_javaJNI.Driver_device_addr_set(swigCPtr, this, player_devaddr_t.getCPtr(value), value);
  }

  public player_devaddr_t getDevice_addr() {
    long cPtr = playercore_javaJNI.Driver_device_addr_get(swigCPtr, this);
    return (cPtr == 0) ? null : new player_devaddr_t(cPtr, false);
  }

  public void setSubscriptions(int value) {
    playercore_javaJNI.Driver_subscriptions_set(swigCPtr, this, value);
  }

  public int getSubscriptions() {
    return playercore_javaJNI.Driver_subscriptions_get(swigCPtr, this);
  }

  public void setEntries(int value) {
    playercore_javaJNI.Driver_entries_set(swigCPtr, this, value);
  }

  public int getEntries() {
    return playercore_javaJNI.Driver_entries_get(swigCPtr, this);
  }

  public void setAlwayson(boolean value) {
    playercore_javaJNI.Driver_alwayson_set(swigCPtr, this, value);
  }

  public boolean getAlwayson() {
    return playercore_javaJNI.Driver_alwayson_get(swigCPtr, this);
  }

  public void setInQueue(QueuePointer value) {
    playercore_javaJNI.Driver_InQueue_set(swigCPtr, this, QueuePointer.getCPtr(value), value);
  }

  public QueuePointer getInQueue() {
    long cPtr = playercore_javaJNI.Driver_InQueue_get(swigCPtr, this);
    return (cPtr == 0) ? null : new QueuePointer(cPtr, false);
  }

  public Driver(ConfigFile cf, int section, boolean overwrite_cmds, long queue_maxlen, int interf) {
    this(playercore_javaJNI.new_Driver__SWIG_0(ConfigFile.getCPtr(cf), cf, section, overwrite_cmds, queue_maxlen, interf), true);
    playercore_javaJNI.Driver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public Driver(ConfigFile cf, int section, boolean overwrite_cmds, long queue_maxlen) {
    this(playercore_javaJNI.new_Driver__SWIG_1(ConfigFile.getCPtr(cf), cf, section, overwrite_cmds, queue_maxlen), true);
    playercore_javaJNI.Driver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public Driver(ConfigFile cf, int section, boolean overwrite_cmds) {
    this(playercore_javaJNI.new_Driver__SWIG_2(ConfigFile.getCPtr(cf), cf, section, overwrite_cmds), true);
    playercore_javaJNI.Driver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public Driver(ConfigFile cf, int section) {
    this(playercore_javaJNI.new_Driver__SWIG_3(ConfigFile.getCPtr(cf), cf, section), true);
    playercore_javaJNI.Driver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public int GetError() {
    return playercore_javaJNI.Driver_GetError(swigCPtr, this);
  }

  public int Subscribe(player_devaddr_t addr) {
    return playercore_javaJNI.Driver_Subscribe__SWIG_0(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr);
  }

  public int Subscribe(QueuePointer queue, player_devaddr_t addr) {
    return playercore_javaJNI.Driver_Subscribe__SWIG_1(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_devaddr_t.getCPtr(addr), addr);
  }

  public int Unsubscribe(player_devaddr_t addr) {
    return playercore_javaJNI.Driver_Unsubscribe__SWIG_0(swigCPtr, this, player_devaddr_t.getCPtr(addr), addr);
  }

  public int Unsubscribe(QueuePointer queue, player_devaddr_t addr) {
    return playercore_javaJNI.Driver_Unsubscribe__SWIG_1(swigCPtr, this, QueuePointer.getCPtr(queue), queue, player_devaddr_t.getCPtr(addr), addr);
  }

  public int Setup() {
    return playercore_javaJNI.Driver_Setup(swigCPtr, this);
  }

  public int Shutdown() {
    return playercore_javaJNI.Driver_Shutdown(swigCPtr, this);
  }

  public void Main() {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Main(swigCPtr, this); else playercore_javaJNI.Driver_MainSwigExplicitDriver(swigCPtr, this);
  }

  public void MainQuit() {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_MainQuit(swigCPtr, this); else playercore_javaJNI.Driver_MainQuitSwigExplicitDriver(swigCPtr, this);
  }

  public void ProcessMessages(int maxmsgs) {
    playercore_javaJNI.Driver_ProcessMessages__SWIG_0(swigCPtr, this, maxmsgs);
  }

  public void ProcessMessages() {
    playercore_javaJNI.Driver_ProcessMessages__SWIG_1(swigCPtr, this);
  }

  public int ProcessMessage(QueuePointer resp_queue, player_msghdr_t hdr, SWIGTYPE_p_void data) {
    return playercore_javaJNI.Driver_ProcessMessage(swigCPtr, this, QueuePointer.getCPtr(resp_queue), resp_queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(data));
  }

  public void Update() {
    if (getClass() == Driver.class) playercore_javaJNI.Driver_Update(swigCPtr, this); else playercore_javaJNI.Driver_UpdateSwigExplicitDriver(swigCPtr, this);
  }

  public int ProcessInternalMessages(QueuePointer resp_queue, player_msghdr_t hdr, SWIGTYPE_p_void data) {
    return (getClass() == Driver.class) ? playercore_javaJNI.Driver_ProcessInternalMessages(swigCPtr, this, QueuePointer.getCPtr(resp_queue), resp_queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(data)) : playercore_javaJNI.Driver_ProcessInternalMessagesSwigExplicitDriver(swigCPtr, this, QueuePointer.getCPtr(resp_queue), resp_queue, player_msghdr_t.getCPtr(hdr), hdr, SWIGTYPE_p_void.getCPtr(data));
  }

  public boolean RegisterProperty(String key, SWIGTYPE_p_Property prop, ConfigFile cf, int section) {
    return (getClass() == Driver.class) ? playercore_javaJNI.Driver_RegisterProperty__SWIG_0(swigCPtr, this, key, SWIGTYPE_p_Property.getCPtr(prop), ConfigFile.getCPtr(cf), cf, section) : playercore_javaJNI.Driver_RegisterPropertySwigExplicitDriver__SWIG_0(swigCPtr, this, key, SWIGTYPE_p_Property.getCPtr(prop), ConfigFile.getCPtr(cf), cf, section);
  }

  public boolean RegisterProperty(SWIGTYPE_p_Property prop, ConfigFile cf, int section) {
    return (getClass() == Driver.class) ? playercore_javaJNI.Driver_RegisterProperty__SWIG_1(swigCPtr, this, SWIGTYPE_p_Property.getCPtr(prop), ConfigFile.getCPtr(cf), cf, section) : playercore_javaJNI.Driver_RegisterPropertySwigExplicitDriver__SWIG_1(swigCPtr, this, SWIGTYPE_p_Property.getCPtr(prop), ConfigFile.getCPtr(cf), cf, section);
  }

}
