package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.JPF;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.util.DynamicObjectArray;
import gov.nasa.jpf.util.StateExtensionClient;
import gov.nasa.jpf.vm.ChoiceGenerator;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.SystemState;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

enum State {
	init,
	locked,
	unlocked
}

class LockAction {
	int threadId;
	State prevState;
	State nextState;
	long lockno;
}

public class LockOrderProperty extends PropertyAdapterBase implements StateExtensionClient<LockAction> {
	
	DynamicObjectArray<LockAction> states = new DynamicObjectArray<LockAction>();
	
	private LockAction lockAction = new LockAction();
	@Override
	public LockAction getStateExtension() {
		LockAction clone = new LockAction();
		clone.threadId = lockAction.threadId;
		clone.lockno = lockAction.lockno;
		clone.nextState = lockAction.nextState;
		clone.prevState = lockAction.prevState;
		return clone;
	}

	@Override
	public void restore(LockAction stateExtension) {
		if (stateExtension.threadId < 0)
			return;
		
		ThreadStorage ts = threadLocks.get(stateExtension.threadId);
		if (ts != null)
			ts.setLockState(stateExtension.lockno, stateExtension.prevState);
	}
	@Override
	public void registerListener(JPF jpf) { }
	
	@Override
	public void stateAdvanced (Search search) {
	    int idx = search.getStateId()+1;
	 
	    LockAction se = this.getStateExtension();
	    states.set(idx, se);
	}

	@Override
	public void stateBacktracked (Search search) {
		int idx = search.getStateId()+1;

		LockAction se = states.get(idx);
		this.restore(se);	
	}

	@Override
	public void stateRestored (Search search) {
		int idx = search.getStateId()+1;
 
		LockAction se = states.get(idx);
		this.restore(se);

	    SystemState ss = search.getVM().getSystemState();
	    ChoiceGenerator<?> cgNext = ss.getNextChoiceGenerator();
	    cgNext.reset();
	}
	
	class ThreadStorage
	{
		private int threadId;
		private Map<Long, State> mutexStates;
		
		public int getId() { return threadId; }
		public boolean hasLockState(long lockno) {
			return mutexStates.containsKey(lockno);
		}
		public State getLockState(long lockno) {
			return mutexStates.get(lockno);
		}
		public void setLockState(long lockno, State state) {
			mutexStates.put(lockno, state);
		}
		
		public void clear() {
			mutexStates.clear();
		}
		
		public ThreadStorage() {
			mutexStates = new HashMap<Long, State>();
		}
	}
	
	private Map<Integer, ThreadStorage>	threadLocks;
	
	private boolean try_acq_rel(ThreadInfo th, long lockno, State nextState) {
		int threadId = th.getId();
		if (!threadLocks.containsKey(threadId))
			threadLocks.put(threadId, new ThreadStorage());
		ThreadStorage ts = threadLocks.get(threadId);
		
		if (!ts.hasLockState(lockno))
			ts.setLockState(lockno, State.init);
		
		if (ts.getLockState(lockno) == nextState)
		{
//			System.out.println("thread#" + threadId + "\t" + nextState + "\t#" + lockno);
//			System.out.println(th.getStackTrace());
			return false;
			
		}
//		if (lockno == 4) {
//			System.out.println("thread#" + threadId + "\t" + nextState + "\t#" + lockno);
//			System.out.println(th.getStackTrace());
//		}
		lockAction.threadId = threadId;
		lockAction.lockno = lockno;
		lockAction.prevState = ts.getLockState(lockno);
		lockAction.nextState = nextState;
		
		ts.setLockState(lockno, nextState);
		return true;
	}
	@Override
	public void reset() {
		super.reset();
		threadLocks.clear();
	}
	
	private void check_acq_rel(ThreadInfo th, Instruction ins, String method, State nextState) {
		InvokeInstruction call = getMethodCall(th, ins, "fse2006daisy.daisy.LockManager", method);
		if (call != null) {
			Object[] args = call.getArgumentValues(th);
			if (args.length == 1) {
				long lockno = (long)args[0];
				if (!try_acq_rel(th, lockno, nextState)) {
					// error: locking and already locked or vice versa
					reportError("Trying to " + method + " " + nextState + " mutex #" + lockno + th.getStackTrace());
				}
			}
		}
	}
	
	@Override
	public void instructionExecuted(VM vm, ThreadInfo th, Instruction nIns, Instruction ins) {

		//lockAction.threadId = -1;
		
		// check whether "ack" is not called on "locked" mutex
		check_acq_rel(th, ins, "acq", State.locked);
		// check whether "rel" is not called on "unlocked" mutex
		check_acq_rel(th, ins, "rel", State.unlocked);
	}
	
	public LockOrderProperty(Config config) {
		this.threadLocks = new HashMap<Integer, LockOrderProperty.ThreadStorage>();
	}
}
