using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using TestCommon;
using TestCommon.Components;

namespace TestClient
{
    public class CommandHandler
    {
#region Singleton definitions
        private static CommandHandler _Instance = new CommandHandler();
        public static CommandHandler Instance
        {
            get { return _Instance; }
        }
        private CommandHandler()
        {
        }
#endregion

        public void OnCommand(string commandLine)
        {
            int i = commandLine.Trim().IndexOf(' ');
            string command = i < 0 ? commandLine : commandLine.Substring(0, i);
            string parameter = i < 0 ? "" : commandLine.Substring(i + 1).Trim();

            MethodInfo methodInfo = this.GetType().GetMethod(command, BindingFlags.IgnoreCase | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
            if (methodInfo == null)
            {
                Reporter.Log(ReportType.Error, "Cannot recognize command '{0}'", command);
                return;
            }

            if (methodInfo.GetParameters().Length > 0)
            {
                methodInfo.Invoke(this, new object[] { parameter });
            }
            else
            {
                methodInfo.Invoke(this, null);
            }
        }

        public void ShowFiles()
        {
            string[] files = Directory.GetFiles(Directory.GetCurrentDirectory());
            Reporter.Log(ReportType.Normal, "Directory : {0}", Directory.GetCurrentDirectory());
            foreach (string filename in files)
            {
                Reporter.Log(ReportType.Normal, "   # {0}", filename);
            }
        }

        private void Clear()
        {
            Reporter.Clear();
        }

        private void Load(string filename)
        {
            if (ComponentController.Instance.Load(filename.Trim()))
            {
                Reporter.Log(ReportType.Result, "Loaded the assembly successfully.");
            }
        }

        private void Unload()
        {
            ComponentController.Instance.UnloadAssembly();
            Reporter.Log(ReportType.Result, "Unloaded the assembly successfully.");
        }

        private void ShowTests()
        {
            ComponentController.Instance.ShowTests();
        }

        private void Run(string command)
        {
            command = command.Trim();
            int dotIndex = command.IndexOf('.');
            if (dotIndex < 0)
            {
                Reporter.Log(ReportType.Error, "Wrong format! USAGE: run <class>.<method> [parameter]");
                return;
            }

            string className = command.Substring(0, dotIndex);
            string methodName = command.Substring(dotIndex + 1);
            int indexOfParameter = methodName.IndexOf(' ');
            string parameter = (indexOfParameter < 0) ? null : methodName.Substring(indexOfParameter).Trim();
            methodName = (indexOfParameter < 0) ? methodName : methodName.Substring(0, indexOfParameter);
            
            ComponentController.Instance.Invoke(className, methodName, parameter);
        }

        private void RunAuto()
        {
            ComponentController.Instance.InvokeAutoRunTest();
        }
    }
}
