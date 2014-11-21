package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

class ThreadStorage
{
	private Map<Long, LockState> mutexStates = new HashMap<Long, LockState>();
	private int threadId;
	public int getThreadId() {
		return threadId;
	}
	
	public LockState getLockState(long lockno) {
		if (!mutexStates.containsKey(lockno))
			mutexStates.put(lockno, LockState.init);
		
		return mutexStates.get(lockno);
	}
	public void setLockState(long lockno, LockState state) {
		mutexStates.put(lockno, state);
	}
	public ThreadStorage(int threadId) {
		this.threadId = threadId;
	}
	
	@Override
	public String toString() {
		return mutexStates.toString();
	}
}
