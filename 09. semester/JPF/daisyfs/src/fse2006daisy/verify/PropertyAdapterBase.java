package fse2006daisy.verify;

import gov.nasa.jpf.PropertyListenerAdapter;
import gov.nasa.jpf.report.Publisher;
import gov.nasa.jpf.search.Search;
import gov.nasa.jpf.vm.Instruction;
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
}
