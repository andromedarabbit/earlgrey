using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CommandLine;
using UnityBuild;

namespace UnityBuild.ConsoleUi
{
    class Program
    {
        private readonly Options _options;

        private Program(Options options)
        {
            Debug.Assert(options != null);
            _options = options;
        }

        static int Main(string[] args)
        {
            Options options = new Options();
            ICommandLineParser parser = new CommandLineParser();
            if (parser.ParseArguments(args, options) == false)
            {
                Console.WriteLine(options.GetUsage());
                return 1;
            }

            if (options.Verbose)
            {
                Console.WriteLine(options.GetSummary());
            }


            Program program = new Program(options);
            return program.Run();
        }

        private string SolutionFilePath
        {
            get
            {
                return Path.GetFullPath(_options.InputFile);
            }
        }

        public int Run()
        {
            Console.WriteLine("working ...");

            try
            {
                BuilderOptions builderOptions = _options.GetBuilderOptions();

                using (Builder builder = new Builder(SolutionFilePath, builderOptions))
                {
                    builder.Open();
                }

                Console.WriteLine("done!");
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine();
                Console.Error.WriteLine("[Error]");
                Console.Error.WriteLine(ex.Message);
                Console.Error.WriteLine();
                return 1;
            }
            
            return 0;
        }
    }
}
