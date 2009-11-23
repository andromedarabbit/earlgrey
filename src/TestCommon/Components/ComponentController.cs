
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Diagnostics;
using TestCommon.Helper;

namespace TestCommon.Components
{
    public class ComponentController
    {
        private Assembly _Assembly = null;
        private AppDomain _AppDomain = null;
        private List<InvokeInformation> _InvokeInformations = new List<InvokeInformation>();
        private bool _DidSetUp = false;

 #region Singleton definitions
        private ComponentController()
        {
        }

        private static ComponentController _Instance = new ComponentController();
        public static ComponentController Instance
        {
            get { return _Instance; }
        }
 #endregion

        public void InvokeSetUp()
        {
            InvokeNoParameterMethod<SetUpAttribute>();
        }

        public void InvokeTearDown()
        {
            if (_DidSetUp)
            {
                InvokeNoParameterMethod<TearDownAttribute>();
            }
        }

        public void InvokeAutoRunTest()
        {
            if (!_DidSetUp)
            {
                InvokeSetUp();
                _DidSetUp = true;
            }
            InvokeNoParameterMethod<TestAttribute>();
        }

        public void InvokeNoParameterMethod<AttributeType>()
        {
            foreach (InvokeInformation invokeInformation in _InvokeInformations)
            {
                if (invokeInformation.Attributes[0] is AttributeType)
                {
                    if (invokeInformation.Method.GetParameters().Length > 0)
                    {
                        continue;
                    }
                    invokeInformation.Method.Invoke(invokeInformation.Instance, null);
                    Reporter.Log(ReportType.Debug, "[{0}] {1}.{2}() has been invoked.", typeof(AttributeType).Name, invokeInformation.Instance.GetType().Name, invokeInformation.Method.Name);
                }
            }
        }

        public void Invoke(string className, string methodName, string parameter)
        {
            if (!_DidSetUp)
            {
                InvokeSetUp();
                _DidSetUp = true;
            }

            foreach (InvokeInformation invokeInformation in _InvokeInformations)
            {
                if (String.Compare(invokeInformation.Instance.GetType().Name, className, true) != 0)
                {
                    continue;
                }
                if (String.Compare(invokeInformation.Method.Name, methodName, true) != 0)
                {
                    continue;
                }
                if (((TestAttribute)invokeInformation.Attributes[0]).TestType == TestType.AutoRun)
                {
                    invokeInformation.Method.Invoke(invokeInformation.Instance, null);
                }
                else
                {
                    try
                    {
                        invokeInformation.Method.Invoke(invokeInformation.Instance, new object[] { parameter });
                    }
                    catch (System.Exception e)
                    {
                        Reporter.Log(ReportType.Error, "Method invocation failed. Exception:{0}", e.Message);
                    }
                }
            }
        }

        public void UnloadAssembly()
        {
            if (_AppDomain != null)
            {
                if (_DidSetUp)
                {
                    InvokeTearDown();
                }
                AppDomain.Unload(_AppDomain);
            }            
            _Assembly = null;
            _AppDomain = null;
            _DidSetUp = false;
        }

        public bool Load(string assembly)
        {
            if (_Assembly != null)
            {
                Debug.Assert(_AppDomain != null);
                UnloadAssembly();
            }

            _AppDomain = AppDomain.CreateDomain("DomainForTest");

            try
            {
                AssemblyName assemblyName = AssemblyName.GetAssemblyName(assembly);
                _Assembly = _AppDomain.Load(assemblyName);
                if (_Assembly == null)
                {
                    Reporter.Log(ReportType.Error, "Can not load the assembly '{0}'", assembly);
                    return false;
                }
            }
            catch (System.Exception e)
            {
                Reporter.Log(ReportType.Error, "Assembly name is wrong. '{0}' Exception:{1}", assembly, e.Message);
                UnloadAssembly();
                return false;
            }

            List<Type> fixtures = new List<Type>();
            AttributeHelper.FindClassesHaveAttribute(_Assembly, typeof(TestFixtureAttribute), fixtures);

            foreach (Type fixture in fixtures)
            {
                AttributeHelper.GetInvokeInformations(fixture, typeof(TestAttribute), _InvokeInformations);
                AttributeHelper.GetInvokeInformations(fixture, typeof(SetUpAttribute), _InvokeInformations);
                AttributeHelper.GetInvokeInformations(fixture, typeof(TearDownAttribute), _InvokeInformations);
            }

            return true;
        }

        public void ShowTests()
        {
            foreach (InvokeInformation information in _InvokeInformations)
            {
                if (information.Attributes[0] is TestAttribute)
                {
                    TestAttribute testAttribute = (TestAttribute)information.Attributes[0];
                    string testType = testAttribute.TestType == TestType.AutoRun ? "(AutoRun)" : "(Interactive)";
                    Reporter.Log(ReportType.Normal, "   # {0}.{1} {2}", information.Instance.GetType().Name, information.Method.Name, testType);
                }
            }
        }
    }
}
