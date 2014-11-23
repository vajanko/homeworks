package fse2006daisy.verify;

import gov.nasa.jpf.Config;
import gov.nasa.jpf.JPF;
import gov.nasa.jpf.listener.PreciseRaceDetector;

public class Verifier {

	public static void main(String[] args) {
		String[] arguments = new String[] 
		{ 
			//"+target=fse2006daisy.DaisyTest",
			//"+target=fse2006daisy.tests.ConcurrentCreat",
			"+target=fse2006daisy.tests.ConcurrentCreat",
			//"+listener=fse2006daisy.verify.CreatPrecondition",
			"+classpath=./bin",
			"+report.publisher=console",
			"+report.console.property_violation=error",
			//"+search.class=gov.nasa.jpf.search.heuristic.BFSHeuristic"
		};
		
		Config conf = JPF.createConfig(arguments);
		
		JPF jpf = new JPF(conf);
		
		//jpf.addPropertyListener(new CreatPrecondition(conf));
		//jpf.addPropertyListener(new LockOrderProperty(conf));
		jpf.addPropertyListener(new PreciseRaceDetector(conf));

		jpf.run();
	}

}
