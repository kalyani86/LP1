import java.util.*;
import java.util.concurrent.Semaphore;

class container
{
    Vector<Integer> buffer=new Vector<>(4);
    static int buffer_size=10;
    static int left=10;
    static Semaphore pro=new Semaphore(1);
    static Semaphore con=new Semaphore(0);
    static Semaphore limit=new Semaphore(buffer_size);

    public void put(int item)
    {
        try
        {
            limit.acquire();
            pro.acquire();

            // if(buffer.size()==buffer.capacity())
            // {
            //     System.out.println("buffer is full");
            //     System.exit(0);
            // }
            left--;
            buffer.add(item);
            System.out.println("Produced "+item);
            System.out.println("Item left to produce "+left);
          
            
            pro.release();
            con.release();

        }
        catch(Exception e)
        {
            System.out.println(e);
        }
    }

    public void get()
    {
        try
        {
             con.acquire();
            pro.acquire();

            if(buffer.size()>0)
            {
                int item=buffer.remove(0);
                System.out.println("consumed "+item+" by "+Thread.currentThread().getName());
            }
            if(left==0)
            {
                System.exit(0);
            }
             pro.release();
             limit.release();
            
        }
        catch(Exception e)
        {
            System.out.println(e);
        }
    }
}
class producer extends Thread
{
    container ct;
    Thread pro;
    
   public producer(container c)
    {
        ct=c;
        pro=new Thread(this,"producer");
        pro.start();
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
        catch(Exception e)
        {
            System.out.println("e"+e);
        }
    }

}

class consumer extends Thread
{
  container ct;
  Thread con;
  static int i=1;
    
   public consumer(container c)
    {
        ct=c;
        con=new Thread(this);
        con.start();
    }

    

    public void run()
    {
        try
        {
            for(i=1;i<=10;i++)
            {
                ct.get();
                Thread.sleep(10);
            }
        }
        catch(Exception e)
        {
            System.out.println("e"+e);
        }
    }
}
class demo
{
    public static void main(String args[])
    {
        container ct=new container();
        producer p=new producer(ct);
        consumer c=new consumer(ct);
        consumer c1=new consumer(ct);
      
    }
}