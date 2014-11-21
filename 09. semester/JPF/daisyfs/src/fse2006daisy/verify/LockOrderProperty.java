package fse2006daisy.verify;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Stack;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.util.DynamicObjectArray;
import gov.nasa.jpf.vm.ChoiceGenerator;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.SystemState;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class LockOrderProperty extends PropertyAdapterBase {
	
	private DynamicObjectArray<LockActionSet> states;
	private ProcessStorage procStorage;
	private LockActionSet currentActionSet;
	private int stateId = 0;
	private Stack<Integer> stateStack = new Stack<Integer>();
	
	private void writeState(String type, int depth, int stateId, int size) {
		if (size == 0)
			return;
		
		depth %= 60;
		for (int i = 0; i < depth; i++)
			System.out.print(" ");
		System.out.println(type + stateId + " (" + size + ")");
	}
	
	@Override
	public void stateProcessed(Search search) {
		//int stateId = search.getStateId() + 2;
		//writeState("=", search.getDepth() + 1, stateId, 0);
	}
	@Override
	public void stateAdvanced (Search search) {
		stateId = search.getStateId() + 1;
		stateStack.push(stateId);
		
		//writeState(">", search.getDepth(), stateId, 0);
		
	    //if (!currentActionSet.empty()) {
	    	states.set(stateId, currentActionSet);
	    	currentActionSet = new LockActionSet();
	    //}
	}
	@Override
	public void stateBacktracked (Search search) {
		//stateId = search.getStateId() + 2;
		stateId = stateStack.pop();
		
		

		LockActionSet sa = states.get(stateId);
		//if (sa != null) {
			procStorage.restore(sa);
			//currentActionSet = sa;
		//}
		//if (!currentActionSet.empty())
			currentActionSet = new LockActionSet();
			
			//writeState("<", search.getDepth() + 1, stateId, sa.size());
	}
	@Override
	public void stateRestored (Search search) {
		stateId = search.getStateId() + 1;
 
		LockActionSet sa = states.get(stateId);
		procStorage.restore(sa);
		currentActionSet = new LockActionSet();

	    SystemState ss = search.getVM().getSystemState();
	    ChoiceGenerator<?> cgNext = ss.getNextChoiceGenerator();
	    cgNext.reset();
	}
	@Override
	public void reset() {
		super.reset();
		procStorage.clear();
		System.out.println("Search reset");
	}
	@Override
	public void searchFinished(Search search) {
		//System.out.println("Search finished");
	}
	/* PropertyAdapterBase overrides */
	
	
	private boolean try_acq_rel(ThreadInfo th, long lockno, LockState nextState) {
		int threadId = th.getId();
		
		LockState currentState = procStorage.getLockState(threadId, lockno);
		if (currentState == nextState)
		{
//			System.out.println("thread#" + threadId + "\t" + nextState + "\t#" + lockno);
//			System.out.println(th.getStackTrace());
			return false;
		}
		currentActionSet.setLockState(threadId, lockno, currentState, nextState);
		
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
