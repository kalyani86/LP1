import java.util.*;
import java.lang.*;
import java.util.concurrent.Semaphore;
class Container
{
    int item;
    Semaphore limit = new Semaphore(100);
    Semaphore prod = new Semaphore(1);
    Semaphore con = new Semaphore(0);
    Vector<Integer>Buffer = new Vector<Integer>(100);

    public void get()
    {
        try
        {
            prod.acquire();
            con.acquire();
            if(Buffer.size() > 0)
            {
                System.out.println("Consumer consumed  item  : " + Buffer.firstElement());
            }

            Buffer.remove(0);
            limit.release();
            prod.release();

        }
        catch(Exception eobj)
        {
            System.out.println("Exception occured : " + eobj);
        }
    }
    public void put(int item)
    {
        try
        {
            prod.acquire();
            limit.acquire();
            this.item = item;

            Buffer.add(item);
            System.out.println("Producer producer item : " + item);

            prod.release();
            con.release();
        }
        catch(Exception eobj)
        {
            System.out.println("Exception occured : " + eobj);
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
        this.t1 = new Thread(this,"Producer");
        t1.start();
    }

    public void run()
    {
        try
        {
            for(int i=1;i<=100;i++)
            {
                pcontain.put(i);
                t1.sleep(4);
            }
        }
        catch(Exception eobj)
        {
            System.out.println("Exception ocuured : " + eobj);
        }
    }

}

class Consumer implements Runnable
{
    Container ccontain;
    Thread t2;
    Consumer(Container c)
    {
        this.ccontain = c;
        this.t2 = new Thread(this,"Consumer");
        t2.start();
    }

    public void run()
    {
        try
        {
            for(int i=1;i<=100;i++)
            {
                ccontain.get();
                t2.sleep(6);
            }
        }
        catch(Exception eobj)
        {
            System.out.println("Exception occured : " + eobj);
        }
    }

}

class mycode
{
    public static void main(String args[])
    {
        Container c = new Container();
        new Producer(c);
        new Consumer(c);
    }
}