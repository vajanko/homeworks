import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class GraphGeneratorImpl 
	extends UnicastRemoteObject
	implements GraphGenerator 
{
	private static final long serialVersionUID = 1L;

	@Override
	public Node CreateNode() throws RemoteException {
		return new RNodeImpl();
	}

	protected GraphGeneratorImpl() throws RemoteException {
		super();
	}
}
