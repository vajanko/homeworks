package fse2006daisy.verify;

public class LockAction {
	public long lockno;
	public LockState prevState;
	public LockState nextState;
	
	@Override
	public String toString() {
		return lockno + ": " + prevState + "->" + nextState;
	}
	
	public LockAction(long lockno, LockState prevState, LockState nextState) {
		this.lockno = lockno;
		this.prevState = prevState;
		this.nextState = nextState;
	}
}
