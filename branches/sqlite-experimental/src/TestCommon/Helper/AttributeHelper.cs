using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Diagnostics;

namespace TestCommon.Helper
{
    public static class AttributeHelper
    {
        public static Assembly LoadAssembly(string name)
        {
            Assembly assembly = null;
            try
            {
                assembly = Assembly.LoadFile(name);
            }
            catch (System.Exception e)
            {
                Reporter.Log(ReportType.System, "Failed to load assembly '{0}'.", name);
                Reporter.Log(ReportType.System, "   Exception - {0}", e.Message);
                return null;
            }

            return assembly;
        }

        public static void FindClassesHaveAttribute(Assembly assembly, Type attributeType, List<Type> container)
        {
            foreach (Type exportedType in assembly.GetExportedTypes())
            {
                object[] attributes = exportedType.GetCustomAttributes(attributeType, false);
                if (attributes == null || attributes.Length == 0)
                {
                    continue;
                }

                container.Add(exportedType);
            }
        }

        public static void FindClassesHaveAttributeOnCurrentDomain(Type attributeType, List<Type> container)
        {
            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
            {
                FindClassesHaveAttribute(assembly, attributeType, container);
            }
        }

        public static void GetInvokeInformations(Type objectType, Type attributeType, List<InvokeInformation> Container)
        {
            object UserObject = Activator.CreateInstance(objectType);
            MethodInfo[] methodInfos = objectType.GetMethods();
            foreach (MethodInfo methodInfo in methodInfos)
            {
                Attribute[] attributes = (Attribute[])methodInfo.GetCustomAttributes(attributeType, false);
                if (attributes == null || attributes.Length == 0)
                {
                    continue;
                }
                InvokeInformation invokeInformation = new InvokeInformation(UserObject, attributes, methodInfo);
                Container.Add(invokeInformation);
            }
        }
    }
}
