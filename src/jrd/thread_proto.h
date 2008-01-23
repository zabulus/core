#ifndef JRD_THREAD_PROTO_H
#define JRD_THREAD_PROTO_H

#include "../common/thd.h"
#include "../jrd/sch_proto.h"

inline void THREAD_ENTER() {
	SCH_enter();
}
inline void THREAD_EXIT() {
	SCH_exit();
}

inline void THREAD_SLEEP(ULONG msecs) {
	THD_sleep(msecs);
}
inline void THREAD_YIELD() {
	THD_yield();
}

class SchedulerContext {
public:
	SchedulerContext()
	{
		SCH_enter();
	}

	~SchedulerContext()
	{
		SCH_exit();
	}

private:
	// copying is prohibited
	SchedulerContext(const SchedulerContext&);
	SchedulerContext& operator=(const SchedulerContext&);
};

#endif // JRD_THREAD_PROTO_H
