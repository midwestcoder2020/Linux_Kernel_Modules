#include  <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/vmstat.h> 


MODULE_LICENSE("GPL"); //*
MODULE_AUTHOR("MWCSUR"); //!
MODULE_DESCRIPTION("Memory monitor module!");//!

#define TIMEOUT 5000

//setup timer for periodic activity
static struct timer_list mem_timer;
int x=0;

static unsigned long total_space=0;
static unsigned long space_left=0;

//setup proc file stuff
//static struct proc_dir_entry *proc_entry;



void mem_timer_callback(struct timer_list * data)
{

    struct sysinfo info;

    // Get memory information
    si_meminfo(&info);

    // Calculate total and free memory in bytes, KB, MB, GB
    unsigned long free_ram = ((info.freeram * info.mem_unit)/1024)/1024;


    unsigned long total_bytes= totalram_pages() * PAGE_SIZE;
    unsigned long total_kb = total_bytes / 1024;
    unsigned long total_ram = total_kb / 1024;


    total_space = total_ram;
    space_left = free_ram;

    printk(KERN_INFO "memory total: %ld mb",total_ram);
    printk(KERN_INFO "memory free: %ld mb",free_ram);

    //check memory and make alert!
    if(space_left<(int)(total_space/10)){
        printk(KERN_WARNING "remaining free memory: %ld mb!",space_left);
        char *argv[]={"/bin/bash","-c", "echo Low Memory Alert! | wall",NULL};
        char * envp[] = { "HOME=/","PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL };
        int res =call_usermodehelper(argv[0],argv, envp,UMH_NO_WAIT);

        if(res !=0){
            printk(KERN_ERR "Unable to make call!");
        }
    }


    mod_timer(&mem_timer,jiffies+msecs_to_jiffies(TIMEOUT));
}

static int mem_module_init(void){
    printk(KERN_INFO "mem_module init: start ....\n");

    //timer setup
    timer_setup(&mem_timer,mem_timer_callback,0);
    mod_timer(&mem_timer,jiffies+msecs_to_jiffies(TIMEOUT));

    printk(KERN_INFO "mem_module init: finished....\n");
    return 0;
}

static void mem_module_exit(void){
    printk(KERN_INFO "mem module cleanup: start.....\n");

    del_timer(&mem_timer);

    //proc_remove(proc_entry);


    printk(KERN_INFO "mem module cleanup: finished.....\n");
}


module_init(mem_module_init);
module_exit(mem_module_exit);