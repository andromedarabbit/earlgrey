using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks
{
    /// <summary>
    /// Show the text message on the console window. You can also color the message.
    /// </summary>
    /// <example>
    /// <code title="Examples" lang="xml" source=".\Samples\msbuild-bettermessage.xml" />
    /// </example>
    /// <remarks></remarks>
    public class BetterMessage : AbstractTask
    {
        /// <summary>
        /// The text messages to be shown on the console windows.
        /// </summary>
        /// <value>Gets or sets the text.</value>
        /// <remarks></remarks>
        public ITaskItem[] Text { get; set; }
   
        /// <summary>
        /// The color of <see cref="Text"/>. For example, 'Red', 'Grean', 'Blue' and etc.
        /// </summary>
        /// <value>Gets or sets the color.</value>
        /// <remarks></remarks>
        public string Color { get; set; }


        /// <summary>
        /// Gets the color of the internal.
        /// </summary>
        /// <remarks></remarks>
        private ConsoleColor InternalColor
        {
            get
            {
                return (ConsoleColor)Enum.Parse(typeof (ConsoleColor), Color, true);
            }
        }

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {            
            return true;
        }

        /// <inheritdoc />
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
