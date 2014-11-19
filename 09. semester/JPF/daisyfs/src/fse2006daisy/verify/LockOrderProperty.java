package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class LockOrderProperty extends PropertyAdapterBase {
	
	enum State {
		locked,
		unlocked
	}
	// 
	private Map<Long, State> mutexStates;
	
	private boolean try_acq_rel(long lockno, State nextState) {
		if (!mutexStates.containsKey(lockno))
			mutexStates.put(lockno, State.unlocked);
		
		if (mutexStates.get(lockno) == nextState)
			return false;
		
		mutexStates.put(lockno, nextState);
		return true;
	}
//	private boolean try_acq(long lockno) {
//		return try_acq_rel(lockno, State.locked);
//	}
//	private boolean try_rel(long lockno) {
//		return try_acq_rel(lockno, State.unlocked);
//	}
	@Override
	public void reset() {
		super.reset();
		mutexStates.clear();
	}
	
	private void check_acq_rel(ThreadInfo th, Instruction ins, String method, State nextState) {
		InvokeInstruction call = getMethodCall(th, ins, "fse2006daisy.daisy.LockManager", method);
		if (call != null) {
			Object[] args = call.getArgumentValues(th);
			long lockno = (long)args[0];
			if (!try_acq_rel(lockno, nextState)) {
				// error: locking and already locked or vice versa
				reportError("Trying to " + method + " " + nextState + " mutex #" + lockno);
			}
		}
	}
	
	@Override
	public void instructionExecuted(VM vm, ThreadInfo th, Instruction nIns, Instruction ins) {
		
		// check whether "ack" is not called on "locked" mutex
		check_acq_rel(th, ins, "ack", State.locked);
		// check whether "rel" is not called on "unlocked" mutex
		check_acq_rel(th, ins, "rel", State.unlocked);
	}
	
	public LockOrderProperty(Config config) {
		this.mutexStates = new HashMap<Long, LockOrderProperty.State>();
	}
}
