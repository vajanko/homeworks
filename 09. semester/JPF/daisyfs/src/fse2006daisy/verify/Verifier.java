package fse2006daisy.verify;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.JPF;

public class Verifier {

	public static void main(String[] args) {
		String[] arguments = new String[] 
		{ 
			"+target=fse2006daisy.DaisyTest",
			"+listener=fse2006daisy.verify.CreatPrecondition",
			"+classpath=./bin",
			"+report.console.property_violation=error,trace"
		};
		
		Config conf = JPF.createConfig(arguments);
		
		JPF jpf = new JPF(conf);
		
		jpf.addPropertyListener(new CreatPrecondition(conf));

		jpf.run();
	}

}
