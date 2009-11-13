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
        public static void FindClassesHaveAttribute(Type attributeType, List<Type> container)
        {
            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
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
        }
    }
}
