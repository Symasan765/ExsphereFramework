#pragma once

#include <functional>

class JobContainer;

class Job final
{
public:
	Job() = default;
	~Job() = default;

	// �W���u�Ƃ��ēo�^���ꂽ�֐������s
	void Execute(float delta_time);

	template<typename T>
	// �W���u�Ƃ��Ď��s����t�@���N�V������ݒ�
	void SetFunction(T* ptr, void (T::* func)(float));

	void SetContainer(JobContainer* container) {
		m_Container = container;
	}
private:
	// �W���u�Ƃ��Ď��s����t�@���N�V�����ϐ�
	std::function<void(float)> m_Function;
	// �o�^�����R���e�i�̃|�C���^
	JobContainer* m_Container = nullptr;

	Job(const Job &) = delete;
	Job &operator=(const Job&) = delete;
};

template<typename T>
inline void Job::SetFunction(T * ptr, void(T::* func)(float))
{
	// bind�@�c�@�w��̊֐������b�v����function���쐬����
	// mem_fn�@�c�@�^����ꂽ�����o�֐����Ăяo�� Callable �I�u�W�F�N�g�𐶐����ĕԂ�
	// placeholders�@�c�@������\��
	m_Function = std::bind(std::mem_fn(func),
		ptr, std::placeholders::_1);
}
