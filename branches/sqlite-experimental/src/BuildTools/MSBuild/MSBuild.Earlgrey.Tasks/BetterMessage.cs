using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks
{
    public class BetterMessage : AbstractTask
    {
        public ITaskItem[] Text { get; set; }
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
            if(Text == null || Text.Length == 0)
            {
                Console.WriteLine();
                return true;
            }

            ConsoleColor oldColor = Console.ForegroundColor;
            try
            {
                if (string.IsNullOrEmpty(Color) == false)
                {
                    Console.ForegroundColor = InternalColor;
                }

                foreach (var taskItem in Text)
                {
                    string item = taskItem.ItemSpec;

                    if (string.IsNullOrEmpty(item))
                    {
                        Console.WriteLine();
                        continue;
                    }

                    Console.WriteLine(item);
                }
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
