#include <napi.h>

#include <iostream>

Napi::Value testSync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  return Napi::Number::New(env, 42);
}

std::condition_variable cond;
std::mutex mutex;
std::atomic_bool done { false };

class TestAsyncWorker : public Napi::AsyncWorker {
public:
  TestAsyncWorker(const Napi::Function& callback) :
      AsyncWorker(callback) {
  }
  void Execute() override {
    std::cout << std::this_thread::get_id() << ": Execute" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
  }
  std::vector<napi_value> GetResult(Napi::Env env) override {
    std::cout << std::this_thread::get_id() << ": GetResult" << std::endl;
    return { Napi::Number::New(env, 42) };
  }
  void Destroy() override {
    std::cout << std::this_thread::get_id() << ": Destroy" << std::endl;
    Napi::AsyncWorker::Destroy();
    done.store(true);
    cond.notify_all();
  }
};

void testAsync(const Napi::CallbackInfo &info) {
  std::cout << std::this_thread::get_id() << ": testAsync" << std::endl;
  Napi::Env env = info.Env();
  auto* worker = new TestAsyncWorker(info[0].As<Napi::Function>());
  worker->Queue();
}

void cleanup(void*) {
  std::cout << std::this_thread::get_id() << ": cleanup" << std::endl;

  std::unique_lock<std::mutex> lock(mutex);
  cond.wait(lock, [] { return done.load(); });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "testSync"), Napi::Function::New(env, testSync));
  exports.Set(Napi::String::New(env, "testAsync"), Napi::Function::New(env, testAsync));

  napi_add_env_cleanup_hook(env, cleanup, nullptr);

  return exports;
}

NODE_API_MODULE(addon, Init)
