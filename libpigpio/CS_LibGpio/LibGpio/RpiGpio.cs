﻿// uncomment the line below to test without libgpio installed
//#define NOPI
using System;
using System.Runtime.InteropServices;
 
namespace LibPiGpio
{
    /*  A basic wrapper for "libpigpio" - a shared library for GPIO access on the Raspberry Pi
     *  To get libpigpio, visit my blog entry here:
     *  http://www.codehosting.net/blog/BlogEngine/post/Raspberry-Pi-Shared-Library-for-GPIO.aspx
    */
 
    public static class RpiGpio
    {
        public static PinSetter Pins { get; set; }
 
#if !NOPI
        [DllImport("libpigpio.so")]
        static extern int setup_io();
        [DllImport("libpigpio.so")]
        static extern void switch_gpio(int val, int pin);
        [DllImport("libpigpio.so")]
        static extern int check_gpio(int pin);
        [DllImport("libpigpio.so")]
        static extern void set_in(int gpio);
        [DllImport("libpigpio.so")]
        static extern void set_out(int gpio);
        [DllImport("libpigpio.so")]
        static extern void rcswitch(int which, int state);
        [DllImport("libpigpio.so")]
        static extern void rsl_switch_decimalcode(int which, int state);
        [DllImport("libpigpio.so")]
        static extern void elro_switch(int which, int state);
        [DllImport("libpigpio.so")]
        static extern void it_switch(int which, int state);
        [DllImport("libpigpio.so")]
        static extern void rev_switch(int which, int state);
        [DllImport("libpigpio.so")]
        static extern void rf_tx_init();
        [DllImport("libpigpio.so")]
        static extern int rf_tx_off();
#else
#region stubs to test without libpigpio
        static void setup_io() { Console.WriteLine("Setup"); }
        static void switch_gpio(int val, int pin) { Console.WriteLine("GPIO{0} set to {1}",pin,val); }
        static int check_gpio(int pin) { Console.WriteLine("GPIO{0} returned 0", pin); return 0; }
        static void set_in(int pin) { Console.WriteLine("GPIO{0} is input", pin); }
        static void set_out(int pin) { Console.WriteLine("GPIO{0} is output", pin); }
#endregion
#endif
 
        static RpiGpio()
        {

        }

        public static int init()
        {
            int rc = 0;
            Pins = new PinSetter();
            rc = setup_io();
            return (rc);
        }

        /// <summary>
        /// switch 433MHz TX off
        /// </summary>

        public static void RFTxInit()
        {
            rf_tx_init();
        }

        public static int RFTxOff()
        {
            int rc = 0;
            rc= rf_tx_off();
            return (rc);
        }

        /// <summary>
        /// Conrad RSL Funk-Steckdosen
        /// </summary>
        /// <param name="which"></param>
        /// <param name="state"></param>

        public static void rslSwitch(int which, int state)
        {
            rsl_switch_decimalcode(which, state);
        }

        public static void elroSwitch(int which, int state)
        {
            elro_switch(which, state);
        }

        public static void revSwitch(int which, int state)
        {
            rev_switch(which, state);
        }

        public static void itSwitch(int which, int state)
        {
            it_switch(which, state);
        }

        public static void SetOutputPins(int[] outputs)
        {
            if (outputs == null || outputs.Length == 0) return;
            foreach (int output in outputs)
                set_out(output);
        }
 
        public static void SetInputPins(int[] inputs)
        {
            if (inputs == null || inputs.Length == 0) return;
            foreach (int input in inputs)
                set_in(input);
        }

        public static int CheckPin(int pin)
        {
            return (check_gpio(pin));

        }

        public class PinSetter
        {
            public bool this[int GpioPin]
            {
                set { switch_gpio(value ? 1 : 0, GpioPin); }
                get { return check_gpio(GpioPin) == 1; }
            }
        }
    }
}

