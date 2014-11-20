package fse2006daisy;

import fse2006daisy.daisy.Petal;

public class DaisyConcurrentTest {

	public static void main(String[] args) {
		Producer pro1 = new Producer("a");
		Producer pro2 = new Producer("b");
		
		Petal.init(false);
		
		pro1.start();
		pro2.start();
	}
}
