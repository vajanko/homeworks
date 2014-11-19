package fse2006daisy.verify;

import java.util.HashMap;
import java.util.Map;

import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class LockOrderProperty extends PropertyAdapterBase {
	
	enum State {
		Locked,
		Unlocked
	}
	// 
	private Map<Long, State> mutexStates;
	
	private boolean try_acq_rel(long lockno, State nextState) {
		if (!mutexStates.containsKey(lockno))
			mutexStates.put(lockno, State.Unlocked);
		
		if (mutexStates.get(lockno) == nextState)
			return false;
		
		mutexStates.put(lockno, nextState);
		return true;
	}
	private boolean try_acq(long lockno) {
		return try_acq_rel(lockno, State.Locked);
	}
	private boolean try_rel(long lockno) {
		return try_acq_rel(lockno, State.Unlocked);
	}
	@Override
	public void reset() {
		super.reset();
		mutexStates.clear();
	}
	
	@Override
	public void instructionExecuted(VM vm, ThreadInfo th, Instruction nIns, Instruction ins) {
		
	}
	@Override
	public void executeInstruction(VM vm, ThreadInfo th, Instruction ins) {
		
	}
	
	public LockOrderProperty() {
		this.mutexStates = new HashMap<Long, LockOrderProperty.State>();
	}
}
