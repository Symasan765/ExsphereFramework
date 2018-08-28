#pragma once
#include <functional>

class AsyncJob final
{
public:
	AsyncJob() = default;
	~AsyncJob() = default;

	// �W���u�Ƃ��ēo�^���ꂽ�֐������s
	void Execute();

	template<typename T>
	void SetFunction(T* ptr, void(T::* func)());

	bool IsFinished()const {
		return m_Finished;
	}
private:
	// �W���u�Ƃ��Ď��s����t�@���N�V�����ϐ�
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
