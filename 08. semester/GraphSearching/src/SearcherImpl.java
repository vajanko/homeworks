import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

class SearcherImpl
	extends UnicastRemoteObject
	implements Searcher
{
	private static final long serialVersionUID = 1L;
	
	@Override
	public int getDistance (Node oFrom, Node oTo) throws RemoteException
	{
		// Implements a trivial distance measurement algorithm.
		// Starting from the source node, a set of visited nodes
		// is always extended by immediate neighbors of all visited
		// nodes, until the target node is visited or no node is left.

		// sVisited keeps the nodes visited in past steps.
		// sBoundary keeps the nodes visited in current step.
		Set<Node> sVisited = new HashSet<Node> ();
		Set<Node> sBoundary = new HashSet<Node> ();
		
		int iDistance = 0;
		
		// We start from the source node.
		sBoundary.add (oFrom);
		
		// Traverse the graph until finding the target node.
		while (!sBoundary.contains (oTo))
		{
			// Not having anything to visit means the target node cannot be reached.
			if (sBoundary.isEmpty ())
				return (Searcher.DISTANCE_INFINITE);
			
			Set<Node> sTraversing = new HashSet<Node> ();
			
			// Nodes visited in current step become nodes visited in past steps.
			sVisited.addAll (sBoundary);
			
			// Collect a set of immediate neighbors of nodes visited in current step.
			for (Node oNode : sBoundary)
				sTraversing.addAll (oNode.getNeighbors ());
			
			// Out of immediate neighbors, consider only those not yet visited.
			for (Iterator<Node> iNode = sTraversing.iterator () ; iNode.hasNext () ;)
			{
				if (sVisited.contains (iNode.next ()))
					iNode.remove ();
			}

			// Make these nodes the new nodes to be visited in current step.
			sBoundary = sTraversing;
			
			iDistance ++;
		}
		
		return (iDistance);
	}
	
	@Override
	public int getTransitiveDistance (int iDistance, Node oFrom, Node oTo) throws RemoteException
	{
		// Implements a transitive distance measurement algorithm.
		// Starting from the source node, a set of visited nodes
		// is always extended by transitive neighbors of all visited
		// nodes, until the target node is visited or no node is left.
		
		// sVisited keeps the nodes visited in past steps.
		// mBoundary keeps the nodes visited in current step.
		Set<Node> sVisited = new HashSet<Node> ();
		Map<Node, Integer> mBoundary = new HashMap<Node, Integer> ();
		
		// We start from the source node.
		mBoundary.put (oFrom, 0);
		
		// Traverse the graph until finding the target node.
		for (;;)
		{
			// Not having anything to visit means the target node cannot be reached.
			if (mBoundary.isEmpty ())
				return (Searcher.DISTANCE_INFINITE);
			
			Map<Node, Integer> mTraversing = new HashMap<Node, Integer> ();
			
			// Collect a set of transitive neighbors of nodes visited in current step.
			for (Entry<Node, Integer> eTuple : mBoundary.entrySet ())
			{
				Node oTuple = eTuple.getKey ();
				if (sVisited.contains (oTuple))
					continue;

				Map<Node, Integer> mPartial = oTuple.getTransitiveNeighbors (iDistance);
				
				for (Entry<Node, Integer> ePartial : mPartial.entrySet ())
				{	
					// Check whether the node is already traversed
					Node oPartial = ePartial.getKey();
					int iDist = eTuple.getValue () + ePartial.getValue ();
					
					if (mTraversing.containsKey (oPartial))
					{
						if (iDist < mTraversing.get (oPartial))
							mTraversing.put (oPartial, iDist);
					}
					else
						mTraversing.put (oPartial, iDist);
				}
				
				// Nodes visited in current step become nodes visited in past steps.
				sVisited.add (oTuple);
			}
			
			for (Entry<Node, Integer> eTuple : mTraversing.entrySet ())
			{
				if (eTuple.getKey().equals(oTo))
					return eTuple.getValue ();
			}
			
			mBoundary = mTraversing;
		}
	}

	public SearcherImpl() throws RemoteException {
		super();
	}
}
