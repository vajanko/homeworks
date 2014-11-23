package fse2006daisy;

import fse2006daisy.daisy.Petal;

public class DaisyConcurrentTest {

	public static void main(String[] args) throws InterruptedException {
		Application pro1 = new Application("aa");
		Application pro2 = new Application("bb");
		
		FSHelper.init();
		
		pro1.start();
		//pro1.run();
		//pro2.start();
		pro2.run();
		
		pro1.join();
		//pro2.join();
	}
}
