package fse2006daisy;

import fse2006daisy.daisy.Petal;

public class DaisyConcurrentTest {

	public static void main(String[] args) throws InterruptedException {
		Producer pro1 = new Producer("a");
		Producer pro2 = new Producer("b");
		
		FSHelper.init();
		
		pro1.start();
		//pro1.run();
		//pro2.start();
		pro2.run();
		
		pro1.join();
		//pro2.join();
	}
}
