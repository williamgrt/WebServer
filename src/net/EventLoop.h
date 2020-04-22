#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <memory>
#include <vector>

namespace gnet {
class Channel;
class Poller;

using ChannelPtr = std::shared_ptr<Channel>;

// 所有eventloop的基类
class EventLoopBase {
public:
  virtual EventLoopBase *GetNextLoop() = 0;
};

// 单线程版本的EventLoop
class EventLoop : public EventLoopBase {
private:
  std::unique_ptr<Poller> poller_;
  std::vector<std::shared_ptr<Channel>>
      channels_; // 所有绑定在该event loop上的channel
  bool loop_;
  bool quit_;
  int maxEvents_;

public:
  EventLoop();
  ~EventLoop();

  void AddChannel(const ChannelPtr &channel);
  void ModifyChannel(const ChannelPtr &channel);
  void DeleteChannel(const ChannelPtr &channel);

  EventLoopBase *GetNextLoop() override { return this; }

  void Loop();
};

} // namespace gnet

#endif // _EVENTLOOP_H