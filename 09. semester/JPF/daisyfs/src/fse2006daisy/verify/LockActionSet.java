package fse2006daisy.verify;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class LockActionSet {
	
	// is this really necessary? isn't is enough to store threadId in the LockActionSet?
	private Map<Integer, List<LockAction>> actions;
	
	ThreadStorage threadStorage;
	
	public Map<Integer, List<LockAction>> getActions() {
		return actions;
	}
	
	public void setLockState(int threadId, long lockno, LockState prevState, LockState nextState) {
		if (!actions.containsKey(threadId))
			actions.put(threadId, new ArrayList<LockAction>());
		
		actions.get(threadId).add(new LockAction(lockno, prevState, nextState));
	}
	
	public boolean empty() {
		return actions.size() == 0;
	}
	public int size() {
		return actions.size();
	}
	
	public LockActionSet() {
		actions = new HashMap<Integer, List<LockAction>>();
	}
	
	@Override
	public String toString() {
		return actions.toString();
	}
}
