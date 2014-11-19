package fse2006daisy.verify;

import fse2006daisy.daisy.DaisyDir;
import gov.nasa.jpf.Config;
import gov.nasa.jpf.GenericProperty;
import gov.nasa.jpf.PropertyListenerAdapter;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.listener.AssertionProperty;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.vm.ClassInfo;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.MethodInfo;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class CreatPrecondition extends PropertyListenerAdapter {
	
	private boolean propHolds = true;
	
	@Override
	public boolean check(Search search, VM vm) {
		return propHolds;
	}
	@Override
	public void reset() {
		propHolds = true;
	}
// TODO: add some nice message telling the user what is going on
//	public String getErrorMessage() {
//		return "create pred";
//	}


	@Override
	public void executeInstruction(VM vm, ThreadInfo currentThread, Instruction instructionToExecute) {

		if (!(instructionToExecute instanceof InvokeInstruction))
			return;
		
		InvokeInstruction call = (InvokeInstruction)instructionToExecute;
		MethodInfo m = call.getInvokedMethod();
		if (m == null)
			return;
		if (!m.getName().equals("creat"))
			return;
		if (m.getArgumentsSize() != 3)
			return;
		
		// TODO: verify that it is a DaisyDir class
		// ClassInfo ci = m.getClassInfo();
		
		Object[] creatArgs = call.getArgumentValues(currentThread);
		propHolds = !creatArgs[0].equals(creatArgs[2]) && creatArgs[0] != null && creatArgs[2] != null;
		
		if (!propHolds) {
			currentThread.skipInstruction(instructionToExecute);
			currentThread.breakTransition();
		}
	}
	
	public CreatPrecondition(Config config) {
		
	}
}
