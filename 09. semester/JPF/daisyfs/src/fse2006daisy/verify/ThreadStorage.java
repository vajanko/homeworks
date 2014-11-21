package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

class ThreadStorage
{
	private Map<Long, LockState> mutexStates;
	
	public LockState getLockState(long lockno) {
		if (!mutexStates.containsKey(lockno))
			mutexStates.put(lockno, LockState.init);
		
		return mutexStates.get(lockno);
	}
	public void setLockState(long lockno, LockState state) {
		mutexStates.put(lockno, state);
	}
	
//	public void clear() {
//		mutexStates.clear();
//	}
	
	public ThreadStorage() {
		mutexStates = new HashMap<Long, LockState>();
	}
	
	@Override
	public String toString() {
		return mutexStates.toString();
	}
}
