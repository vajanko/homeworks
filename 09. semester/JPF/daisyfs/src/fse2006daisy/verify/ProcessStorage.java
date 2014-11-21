package fse2006daisy.verify;

import java.awt.Desktop.Action;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import fse2006daisy.util.Assert;

public class ProcessStorage {
	
	private Map<Integer, ThreadStorage> threadStates;
	
	private ThreadStorage getThreadStorage(int threadId) {
		if (!threadStates.containsKey(threadId))
			threadStates.put(threadId, new ThreadStorage());
		
		return threadStates.get(threadId);
	}
	public LockState getLockState(int threadId, long lockno) {
		return getThreadStorage(threadId).getLockState(lockno);
	}
	public void setLockState(int threadId, long lockno, LockState state) {
		getThreadStorage(threadId).setLockState(lockno, state);
	}
	public void clear() {
		threadStates.clear();
	}
	
	public ProcessStorage() {
		threadStates = new HashMap<Integer, ThreadStorage>();
	}
	public void restore(LockActionSet actionSet) {
		
		if (actionSet.getActions().size() > 1) {
			System.out.println("multiple action sets");
		}
		
		for (Entry<Integer, List<LockAction>> entry : actionSet.getActions().entrySet()) {
			List<LockAction> actions = entry.getValue();
			int threadId = entry.getKey();
			//for (LockAction act : actions) {
			// process in reverse order
			for (int i = actions.size() - 1; i>=0; i--) {
				LockAction act = actions.get(i);
				ThreadStorage ts = getThreadStorage(threadId);
				if (getLockState(threadId, act.lockno) != act.nextState) {
					System.out.println("revert incorrect");
				}
				setLockState(threadId, act.lockno, act.prevState);
			}
		}
		
	}
	
	@Override
	public String toString() {
		return threadStates.toString();
	}
}
