import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Searcher extends Remote 
{
	public static final int DISTANCE_INFINITE = -1;
	public int getDistance (Node oFrom, Node oTo) throws RemoteException;
	public int getTransitiveDistance (int iDistance, Node oFrom, Node oTo) throws RemoteException;	
}

