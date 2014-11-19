package gov.nasa.jpf;

import gov.nasa.jpf.PropertyListenerAdapter;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class CreatPrecondition extends PropertyListenerAdapter {
	
	@Override
	public void instructionExecuted(VM vm, ThreadInfo currentThread,
			Instruction nextInstruction, Instruction executedInstruction) {
		// TODO Auto-generated method stub
		super.instructionExecuted(vm, currentThread, nextInstruction,
				executedInstruction);
	}
	
	public CreatPrecondition() {

	}
}
