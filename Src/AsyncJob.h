#pragma once
#include <functional>

class AsyncJob final
{
public:
	AsyncJob() = default;
	~AsyncJob() = default;

	// ジョブとして登録された関数を実行
	void Execute();

	template<typename T>
	void SetFunction(T* ptr, void(T::* func)());

	bool IsFinished()const {
		return m_Finished;
	}
private:
	// ジョブとして実行するファンクション変数
	std::function<void(void)> m_Function;
	bool m_Finished = false;

	AsyncJob(const AsyncJob &) = delete;
	AsyncJob &operator=(const AsyncJob &) = delete;
};

template<typename T>
inline void AsyncJob::SetFunction(T * ptr, void(T::* func)())
{
	m_Function = std::bind(std::mem_fn(func), ptr);
}
