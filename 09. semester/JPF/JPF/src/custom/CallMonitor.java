import gov.nasa.jpf.ListenerAdapter;
import gov.nasa.jpf.jvm.bytecode.InvokeInstruction;
import gov.nasa.jpf.vm.Instruction;
import gov.nasa.jpf.vm.MethodInfo;
import gov.nasa.jpf.vm.ThreadInfo;
import gov.nasa.jpf.vm.VM;

public class CallMonitor extends ListenerAdapter {

	@Override
	public void executeInstruction(VM vm, ThreadInfo currentThread, Instruction instructionToExecute) {
		
		System.out.println("hello");
		
		if (instructionToExecute instanceof InvokeInstruction) {
			InvokeInstruction ins = (InvokeInstruction)instructionToExecute;
			MethodInfo m = ins.getInvokedMethod();
			
			System.out.println("Invoking method: " + m.getName());
		}
	}
}
