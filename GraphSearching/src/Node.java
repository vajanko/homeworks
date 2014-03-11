import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Map;
import java.util.Set;

public interface Node extends Remote
{
	Set<Node> getNeighbors () throws RemoteException;
	Map<Node, Integer> getTransitiveNeighbors (int iDistance) throws RemoteException;
	void addNeighbor (Node oNeighbor)throws RemoteException;
}
