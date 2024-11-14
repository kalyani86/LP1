import java.util.concurrent.Semaphore;
import java.util.Vector;
import java.lang.Thread;
import java.lang.Runnable;

class Container{
    Vector<Integer> Buffer = new Vector<Integer>(100);
    static Semaphore limit = new Semaphore(100);
    static Semaphore Prod = new Semaphore(1);
    static Semaphore Con = new Semaphore(0);
    int item;
    void get()
    {
        try{
            Con.acquire();
            Prod.acquire();

            if(Buffer.size() > 0)
            {
                System.out.println("Consumer consumed " + Buffer.size() + "number of items");
                System.out.println("Number of items left : "+ Prod.availablePermits());
                System.out.println();

                for(int i=0;i<Buffer.size();i++)
                {
                    limit.release();

                }
                Buffer.clear();
            }

            Prod.release();

        }
        catch(Exception eobj)
        {

        }
    }
    void put(int item)
    {
        try{
            limit.acquire();
            Prod.acquire();

            this.item = item;
            Buffer.add(item);
            System.out.println("Producer produced : " + item);
            System.out.println("Total items produced by producer : " + Buffer.size());
            System.out.println("Number of items producer can still producer : " + limit.availablePermits());
            Prod.release();
            Con.release();
        }
        catch(Exception eobj)
        {
            System.out.println(eobj);
        }
        
    }
}

class Producer implements Runnable
{
    Container pcontain;
    Thread t1;
    Producer(Container c)
    {
        this.pcontain = c;
        t1 = new Thread(this,"Producer");
        t1.start();
    }
    public void run()
    {
        try
        {
            for(int i=1;i<=100;i++)
            {
                pcontain.put(i);
                Thread.sleep(5);
            }
        }
        catch(Exception eobj)
        {
            System.out.println(eobj);
        }
    }
}

class Consumer implements Runnable{
    Container ccontain;
    Thread t2;
    Consumer(Container c)
    {
        this.ccontain = c;
        t2 = new Thread(this,"Consumer");
        t2.start();
    }
    public void run()
    {
        try
        {
            for(int i=1;i<=100;i++)
            {
                ccontain.get();
                Thread.sleep(30);
            }
        }catch(Exception obj)
        {
            System.out.println(obj);
        }
    }
}

public class code{
    public static void main(String args[])
    {
        Container c = new Container();
        new Producer(c);
        new Consumer(c);
    }
}