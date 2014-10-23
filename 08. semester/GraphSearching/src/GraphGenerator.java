import java.rmi.Remote;
import java.rmi.RemoteException;

public interface GraphGenerator extends Remote 
{
	Node CreateNode() throws RemoteException;
}
