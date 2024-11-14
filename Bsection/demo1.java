import java.util.concurrent.Semaphore;
import java.util.Vector;
import java.lang.Thread;
import java.lang.Runnable;

class Container
{
    Vector<Integer>buffer = new Vector<>(10);
    static final int buffer_size = 10;

    static Semaphore con = new Semaphore(0);   //counts filled slots
    static Semaphore prod = new Semaphore(1);  //currently free resource --> mutex if prod(0)
    static Semaphore empty_slots = new Semaphore(buffer_size);  //count empty slots

    //Here, prod - binary semaphore(initialized with value (1) and takes only 2 values 0 or 1 indicating mutex conditions) 
    // con,empty_slots - counting semaphores 

    void put(int item)
    {
        try
        {
            prod.acquire();  //mutex applied -> resource is locked by only one thread -> set prod(0)
            empty_slots.acquire();  //wait for empty slots  //decreses count of empty_slots by one as producer is producing

            buffer.add(item);
            System.out.println("\nProducer produced item: "+item);
            System.out.println("No of items produced: "+buffer.size());
            System.out.println("No of items producer can still produced: "+empty_slots.availablePermits());

            prod.release(); //prod(1) -> resource is availatble
            con.release(); //signaling that consumer can consume now

        }
        catch(InterruptedException e)
        {
            System.out.println("Exception: "+e);
        }
    }
    
    void get()
    {
        try
        {
            prod.acquire();
            con.acquire();

            if(buffer.size() > 0)
            {
                int item = buffer.remove(0);
                System.out.println("\nItem consumed is: "+item);  //removes 0th index element
                System.out.println("No of itmes left in buffer: "+buffer.size());
            }

            prod.release();
            empty_slots.release();   //increase no of empty_slots semaphore value by 1
        }
        catch(InterruptedException e)
        {
            System.out.println("\nException: "+e);
        }
    }

}

class Producer implements Runnable
{
    Container ct;
    Thread t;
    Producer(Container ct)
    {
        this.ct = ct;
        t = new Thread(this,"Producer");
        t.start();
    }
    public void run()
    {
        try
        {
            for(int i=1;i<=10;i++)
            {
                ct.put(i);
                Thread.sleep(5);
            }
        }
        catch(InterruptedException e)
        {
            System.out.println("Exception: "+e);
        }
    }
}

class Consumer implements Runnable
{
    Container ct;
    Thread t;
    Consumer(Container ct)
    {
        this.ct = ct;
        t = new Thread(this,"Consumer");
        t.start();
    }
    public void run()
    {
        try
        {
            for(int i=1;i<=10;i++)
            {
                ct.get();
                Thread.sleep(30);
            }
        }
        catch(InterruptedException e)
        {
            System.out.println("Exception: "+e);
        }
    }
}

class demo1
{
    public static void main(String [] args)
    {
        Container container = new Container();
        Producer producer = new Producer(container);
        Consumer consumer = new Consumer(container);
    }
}