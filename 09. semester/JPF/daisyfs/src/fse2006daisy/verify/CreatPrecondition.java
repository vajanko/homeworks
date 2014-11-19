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

public class CreatPrecondition extends PropertyAdapterBase {
	
	private String getPrecondMessage(String clazz, String method, String cond) {
		return (clazz != null ? clazz + "." : "") + method + " precondition property " + cond + " violated";
	}

	@Override
	public void executeInstruction(VM vm, ThreadInfo th, Instruction ins) {

		if (!(ins instanceof InvokeInstruction))
			return;
		
		InvokeInstruction call = (InvokeInstruction)ins;
		MethodInfo m = call.getInvokedMethod();
		if (m == null)
			return;
		if (!m.getName().equals("creat"))
			return;
		if (m.getArgumentsSize() != 3)
			return;
		
		// TODO: verify that it is a DaisyDir class
		ClassInfo ci = m.getClassInfo();
		
		Object[] creatArgs = call.getArgumentValues(th);
		
		if (creatArgs[0].equals(creatArgs[2]))
			reportError(getPrecondMessage(ci.getName(), m.getName(), "dir != fh"));
		if (creatArgs[0] == null)
			reportError(getPrecondMessage(ci.getName(), m.getName(), "dir != null"));
		if (creatArgs[2] == null)
			reportError(getPrecondMessage(ci.getName(), m.getName(), "fh != null"));
		
		// if any
		skipError(th, ins);
	}
	
	public CreatPrecondition(Config config) {
		
	}
}
