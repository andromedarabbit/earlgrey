using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Xml;

namespace UnityBuild
{
    public class PrecompiledHeaderOptions
    {
        private UsePrecompiledHeaderOptions _usePrecompiledHeader;
        private string _precompiledHeaderThrough;
        private string _precompiledHeaderFile;

        public PrecompiledHeaderOptions()
            : this(UsePrecompiledHeaderOptions.None)
        {
            
        }

        public PrecompiledHeaderOptions(UsePrecompiledHeaderOptions usePrecompiledHeader)
        {
            this._usePrecompiledHeader = usePrecompiledHeader;
            _precompiledHeaderThrough = string.Empty;
            _precompiledHeaderFile = string.Empty;
        }

        public UsePrecompiledHeaderOptions UsePrecompiledHeader
        {
            get { return _usePrecompiledHeader; }
            set { _usePrecompiledHeader = value; }
        }

        public string PrecompiledHeaderThrough
        {
            get { return _precompiledHeaderThrough; }
            set { _precompiledHeaderThrough = value; }
        }

        public string PrecompiledHeaderFile
        {
            get { return _precompiledHeaderFile; }
            set { _precompiledHeaderFile = value; }
        }

        
        public void FillConfigurationTypeTool(ConfigurationTypeTool tool)
        {
            Debug.Assert(tool.Name == "VCCLCompilerTool");
            Debug.Assert(tool.NameSpecified == true);
            Debug.Assert(tool.AnyAttr != null);

            // tool.AnyAttrSpecified = true;
            
            if( _usePrecompiledHeader == UsePrecompiledHeaderOptions.InheritFromProject )
            {
                tool.AnyAttr.Clear();
                tool.AnyAttrSpecified = false;
                return;
            }

            tool.AnyAttrSpecified = true;

            //if( GetUsePrecompiledHeader(tool.AnyAttr) == UsePrecompiledHeaderOptions.InheritFromProject )
            //{
                
                
            //}

            throw new NotImplementedException();
        }
        
        public static PrecompiledHeaderOptions CreateInstance(ConfigurationTypeTool tool)
        {
            Debug.Assert(tool.Name == "VCCLCompilerTool");
            Debug.Assert(tool.NameSpecified == true);
            Debug.Assert(tool.AnyAttr != null);

            //if (tool.AnyAttr.Count == 0)
            if(tool.UsePrecompiledHeaderSpecified == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            PrecompiledHeaderOptions options = new PrecompiledHeaderOptions();
            options.UsePrecompiledHeader = (UsePrecompiledHeaderOptions)Enum.Parse(typeof(UsePrecompiledHeaderOptions), tool.UsePrecompiledHeader);// GetUsePrecompiledHeader(tool.AnyAttr);
            options.PrecompiledHeaderThrough = tool.PrecompiledHeaderThrough; // GetPrecompiledHeaderThrough(tool.AnyAttr);
            options.PrecompiledHeaderFile = tool.PrecompiledHeaderFile; // GetPrecompiledHeaderFile(tool.AnyAttr);

            return options;
        }
        /*
        private static UsePrecompiledHeaderOptions GetUsePrecompiledHeader(List<XmlAttribute> attributes)
        {
            int indexFound = attributes.FindIndex(
                xmlAttribute => xmlAttribute.Name == "UsePrecompiledHeader"
                );

            if (indexFound < 0)
                return UsePrecompiledHeaderOptions.InheritFromProject;

            return (UsePrecompiledHeaderOptions)int.Parse(
                attributes[indexFound].Value
                );
        }

        private static string GetPrecompiledHeaderThrough(List<XmlAttribute> attributes)
        {
            return GetStringValueFromAttributes(attributes, "PrecompiledHeaderThrough");
        }

        private static string GetPrecompiledHeaderFile(List<XmlAttribute> attributes)
        {
            return GetStringValueFromAttributes(attributes, "PrecompiledHeaderFile");
        }

        private static string GetStringValueFromAttributes(List<XmlAttribute> attributes, string key)
        {
            int indexFound = attributes.FindIndex(
                xmlAttribute => xmlAttribute.Name == key
                );

            if (indexFound < 0)
                return string.Empty;

            return attributes[indexFound].Value;
        }
        */
    }
}
