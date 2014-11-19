package fse2006daisy.verify;

import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class LockOrderProperty extends PropertyAdapterBase {
	
	@Override
	public void instructionExecuted(VM vm, ThreadInfo th, Instruction nIns, Instruction ins) {
		
	}
	@Override
	public void executeInstruction(VM vm, ThreadInfo th, Instruction ins) {
		
	}
	
	public LockOrderProperty() {
		
	}
}
