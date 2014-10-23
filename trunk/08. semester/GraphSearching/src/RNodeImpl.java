import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class RNodeImpl
	extends UnicastRemoteObject
	implements Node
{
	private static final long serialVersionUID = 1L;
	
	private final Set<Node> sNodes = new HashSet<Node> ();

	@Override
	public Set<Node> getNeighbors() throws RemoteException {
		return sNodes;
	}

	@Override
	public Map<Node, Integer> getTransitiveNeighbors(int iDistance)
			throws RemoteException {
		
		if (iDistance <= 0)
			throw new IllegalArgumentException ("Argument distance must be positive");
			
		Map<Node, Integer> mResult = new HashMap<Node, Integer> ();
		Set<Node> sCurLayer = new HashSet<Node> ();
		
		// Initial zero-distance node
		sCurLayer.add (this);
		
		// Closure for each level of i-distant nodes
		for (int i = 0 ; i < iDistance ; ++i)
		{
			Set<Node> sNextLayer = new HashSet<Node> ();
			
			// Use nodes which are in the current level
			for (Node oNode : sCurLayer)
			{
				if (!mResult.containsKey (oNode))
				{
					mResult.put (oNode, i);
					sNextLayer.addAll (oNode.getNeighbors ());
				}
			}
			
			// Move to the next layer
			sCurLayer = sNextLayer;
		}
		
		// Handle the last layer
		for (Node oNode : sCurLayer)
		{
			if (!mResult.containsKey (oNode))
				mResult.put (oNode, iDistance);
		}
		
		return mResult;
	}

	@Override
	public void addNeighbor(Node oNeighbor) throws RemoteException {
		sNodes.add (oNeighbor);
	}
	
	public RNodeImpl() throws RemoteException {
		super();
	}

}
