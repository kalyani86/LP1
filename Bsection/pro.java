/*
Q4.WAP to check interthread communiaction using producer and consumer example.
*/

class Container
{
	int data;
	int flag=0;
	synchronized public void produce(int i)
	{
		data=i;
		System.out.println("produced data "+data);
		flag=1;
		try
		{
			notifyAll();
			wait();
		}
		catch(Exception e)
		{
			System.out.println("caught");
		}
	}
	
	synchronized public void consume()
	{
		System.out.println("consumed data "+data);
		flag=0;
		try
		{
			notifyAll();
			wait();
		}
		catch(Exception e)
		{
			System.out.println("caught");
		}
	}
}

class Producer extends Thread
{
	Container ct;
	Thread t;
	Producer(Container c)
	{
		ct=c;
		t=new Thread(this,"Producer");
	}
	void startThread()
	{
		t.start();
	}
	public void run()
	{
		int i=1;
		while(true)
		{
			if(ct.flag==0)
			{
				ct.produce(i);
			    i++;
			}
			else
			{
				System.out.println("Producer polling");
			}
			
		}
	}
}

class Consumer extends Thread
{
	Container ct;
	Thread t;
	Consumer(Container c)
	{
		ct=c;
		t=new Thread(this,"consumer");
	}
	
	void startThread()
	{
		t.start();
	}
	public void run()
	{
		while(true)
		{
			if(ct.flag==1)
			{
				ct.consume();
			}
			else
			{
				System.out.println("Consumer polling");
			}
		}
		
	}
}

class pro
{
	public static void main(String args[]) throws InterruptedException
	{
		Container ct=new Container();
		Producer p=new Producer(ct);
		Consumer c=new Consumer(ct);
		p.startThread();
		c.startThread();
		Thread.sleep(100);
		System.exit(0);
	}
}