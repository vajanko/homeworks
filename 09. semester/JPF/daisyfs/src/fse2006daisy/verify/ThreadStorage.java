package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

class ThreadStorage
{
	private Map<Long, LockState> mutexStates = new HashMap<Long, LockState>();
	private int threadId;
	public int getThreadId() { return threadId; }
	public void setThreadId(int threadId) { this.threadId = threadId; }
	
	public Map<Long, LockState> getMutextStates() {
		return mutexStates;
	}
	public boolean empty() {
		return mutexStates.size() == 0;
	}
	
	public LockState getLockState(long lockno) {
		if (!mutexStates.containsKey(lockno))
			mutexStates.put(lockno, LockState.init);
		
		return mutexStates.get(lockno);
	}
	public void setLockState(long lockno, LockState state) {
		mutexStates.put(lockno, state);
	}
	public void setLockStateOnce(int threadId, long lockno, LockState state) {
		if (!mutexStates.containsKey(lockno)) {
			this.threadId = threadId;
			mutexStates.put(lockno, state);
		}
	}
	public ThreadStorage() {
		this.threadId = -1;
	}
	
	@Override
	public String toString() {
		return mutexStates.toString();
	}
}
