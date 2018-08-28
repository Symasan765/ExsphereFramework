#pragma once

#include <functional>

class JobContainer;

class Job final
{
public:
	Job() = default;
	~Job() = default;

	// ジョブとして登録された関数を実行
	void Execute(uint64_t delta_time);

	template<typename T>
	// ジョブとして実行するファンクションを設定
	void SetFunction(T* ptr, void (T::* func)(uint64_t));

	void SetContainer(JobContainer* container) {
		m_Container = container;
	}
private:
	// ジョブとして実行するファンクション変数
	std::function<void(uint64_t)> m_Function;
	// 登録したコンテナのポインタ
	JobContainer* m_Container = nullptr;

	Job(const Job &) = delete;
	Job &operator=(const Job&) = delete;
};

template<typename T>
inline void Job::SetFunction(T * ptr, void(T::* func)(uint64_t))
{
	// bind　…　指定の関数をラップしてfunctionを作成する
	// mem_fn　…　与えられたメンバ関数を呼び出す Callable オブジェクトを生成して返す
	// placeholders　…　引数を表す
	m_Function = std::bind(std::mem_fn(func),
		ptr, std::placeholders::_1);
}
