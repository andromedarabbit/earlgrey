using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks
{
    public class BetterMessage : AbstractTask
    {
        public string Text { get; set; }
        public string Color { get; set; }


        private ConsoleColor InternalColor
        {
            get
            {
                return (ConsoleColor)Enum.Parse(typeof (ConsoleColor), Color, true);
            }
        }

        protected override bool ValidateParameters()
        {            
            return true;
        }

        protected override bool ExecuteCommand()
        {
            if (string.IsNullOrEmpty(Text))
            {
                Console.WriteLine();
                return true;
            }

            if (string.IsNullOrEmpty(Color))
            {
                Console.WriteLine(Text);
                return true;
            }

            ConsoleColor oldColor = Console.ForegroundColor;
            try
            {
                Console.ForegroundColor = InternalColor;
                Console.WriteLine(Text);
            }
            catch (ArgumentException argEx)
            {
                Log.LogErrorFromException(argEx);
                return false;
            }
            finally
            {
                Console.ForegroundColor = oldColor;
            }
            
            return true;
        }
    }

}
