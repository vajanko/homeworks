package fse2006daisy.verify;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.MethodInfo;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class CreatPrecondition extends PropertyAdapterBase {
	
	private String getPrecondMessage(String clazz, String method, String cond) {
		return (clazz != null ? clazz + "." : "") + method + "(...) \"" + cond + "\" precondition violated";
	}
	private void reportPrecondError(InvokeInstruction methodCall, String cond) {
		MethodInfo m = methodCall.getInvokedMethod();
		reportError(getPrecondMessage(m.getClassInfo().getName(), m.getName(), cond));
	}

	@Override
	public void executeInstruction(VM vm, ThreadInfo th, Instruction ins) {
	
		InvokeInstruction call = getMethodCall(th, ins, "fse2006daisy.daisy.DaisyDir", "creat");
		if (call != null) {
			Object[] args = call.getArgumentValues(th);
		
			if (args[0].equals(args[2]))
				reportPrecondError(call, "dir != fh");
			if (args[0] == null)
				reportPrecondError(call, "dir != null");
			if (args[2] == null)
				reportPrecondError(call, "fh != null");
			
			skipError(th, ins);		// if any
		}
	}
	
	public CreatPrecondition(Config config) {
		
	}
}
