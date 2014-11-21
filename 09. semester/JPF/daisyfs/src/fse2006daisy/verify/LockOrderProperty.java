package fse2006daisy.verify;

import java.util.Stack;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.util.DynamicObjectArray;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class LockOrderProperty extends PropertyAdapterBase {
	
	private DynamicObjectArray<LockActionSet> states;
	
	private ProcessStorage procStorage;
	// lock actions (ACQ/REL) discovered in current state
	private LockActionSet currentActionSet;
	// unique identifier of currently searched state
	private int stateId = 0;
	private Stack<Integer> stateStack = new Stack<Integer>();
	
	@Override
	public void stateAdvanced (Search search) {
		stateId = search.getStateId();
		stateStack.push(stateId);
			
	    //if (!currentActionSet.empty()) {
	    	states.set(stateId, currentActionSet);
	    	currentActionSet = new LockActionSet();
	    //}
	}
	@Override
	public void stateBacktracked (Search search) {
		stateId = stateStack.pop();

		LockActionSet sa = states.get(stateId);
		//if (sa != null)
			procStorage.restore(sa);
		//if (!currentActionSet.empty())
		//	currentActionSet = new LockActionSet();
	}	
	
	private boolean try_acq_rel(ThreadInfo th, long lockno, LockState nextState) {
		int threadId = th.getId();
		
		LockState currentState = procStorage.getLockState(threadId, lockno);
		if (currentState == nextState)
			return false;	// trying to ACQ already locked or REL already unlocked mutex
		
		// remember this action in the state history ...
		currentActionSet.setLockState(threadId, lockno, currentState, nextState);
		// ... as well as in the current process state
		procStorage.setLockState(threadId, lockno, nextState);
		
		return true;
	}
	
	private void check_acq_rel(ThreadInfo th, Instruction ins, String method, LockState nextState) {
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
		
		// check whether "ack" is not called on "locked" mutex
		check_acq_rel(th, ins, "acq", LockState.locked);
		// check whether "rel" is not called on "unlocked" mutex
		check_acq_rel(th, ins, "rel", LockState.unlocked);
	}
	
	public LockOrderProperty(Config config) {
		this.procStorage = new ProcessStorage();
		this.states = new DynamicObjectArray<LockActionSet>();
		this.currentActionSet = new LockActionSet();
	}
	
	
}
