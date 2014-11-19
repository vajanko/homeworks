package fse2006daisy.verify;

import gov.nasa.jpf.PropertyListenerAdapter;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.report.Publisher;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.vm.ClassInfo;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.MethodInfo;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class PropertyAdapterBase extends PropertyListenerAdapter {
	
	protected String message = null;
	
	protected boolean hasError() {
		return message != null;
	}
	
	@Override
	public boolean check(Search search, VM vm) {
		return !hasError();
	}
	@Override
	public void reset() {
		message = null;
	}
	@Override
	public String getErrorMessage() {
		return message;
	}
	protected void reportError(String msg) {
		if (this.message == null)
			this.message = msg;
		else
			this.message += msg;
	}
	protected void skipError(ThreadInfo th, Instruction ins) {
		if (hasError()) {
			th.skipInstruction(ins);
			th.breakTransition();
		}
	}
	protected InvokeInstruction getMethodCall(ThreadInfo th, Instruction ins, String clazz, String method) {

		if (ins instanceof InvokeInstruction) {
			if (/*ins.isCompleted(th) &&*/ !th.isInstructionSkipped()) {
				InvokeInstruction call = (InvokeInstruction) ins;
				MethodInfo mi = call.getInvokedMethod();
				//Object[] args = call.getArgumentValues(th);
				ClassInfo ci = mi.getClassInfo();

				if (ci != null && ci.getName().equals(clazz) && mi.getName().equals(method))
					return call;
			}
		}
		
		return null;
	}
}
