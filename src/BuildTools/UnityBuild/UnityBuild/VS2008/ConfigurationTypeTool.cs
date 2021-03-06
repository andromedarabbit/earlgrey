﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace UnityBuild.VS2008
{
    public partial class ConfigurationTypeTool
    {
        private string _usePrecompiledHeader;
        private bool _usePrecompiledHeaderSpecified;

        private string _precompiledHeaderFile;
        private bool _precompiledHeaderFileSpecified;

        private string _precompiledHeaderThrough;
        private bool _precompiledHeaderThroughSpecified;

        private string _preprocessorDefinitions;
        private bool _preprocessorDefinitionsSpecified;

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string UsePrecompiledHeader
        {
            get
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");
                if (this._usePrecompiledHeader == null)
                    this._usePrecompiledHeader = string.Empty;
                return this._usePrecompiledHeader;
            }
            set
            {
                if ((this._usePrecompiledHeader != null))
                {
                    Debug.Assert(this.nameField == "VCCLCompilerTool");

                    if ((_usePrecompiledHeader.Equals(value) != true))
                    {
                        this._usePrecompiledHeader = value;
                        UsePrecompiledHeaderSpecified = UsePrecompiledHeaderOption !=
                                                        UsePrecompiledHeaderOptions.InheritFromProject;

                        this.OnPropertyChanged("UsePrecompiledHeader");
                    }
                }
                else // will be used by a serializer
                {
                    this._usePrecompiledHeader = value;
                    UsePrecompiledHeaderSpecified = UsePrecompiledHeaderOption !=
                                                    UsePrecompiledHeaderOptions.InheritFromProject;
                    this.OnPropertyChanged("UsePrecompiledHeader");
                }
            }
        }

        [XmlIgnore()]
        public UsePrecompiledHeaderOptions UsePrecompiledHeaderOption
        {
            get
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");

                return (UsePrecompiledHeaderOptions) Enum.Parse(
                    typeof (UsePrecompiledHeaderOptions), _usePrecompiledHeader
                                                         );
            }
        }

        [XmlIgnore()]
        public bool UsePrecompiledHeaderSpecified
        {
            get
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                return _usePrecompiledHeaderSpecified;
            }
            set
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                _usePrecompiledHeaderSpecified = value;
            }
        }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string PrecompiledHeaderFile
        {
            get
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");

                if (this._precompiledHeaderFile == null)
                    this._precompiledHeaderFile = string.Empty;
                return this._precompiledHeaderFile;
            }
            set
            {
                if ((this._precompiledHeaderFile != null))
                {
                    Debug.Assert(this.nameField == "VCCLCompilerTool");

                    if ((_precompiledHeaderFile.Equals(value) != true))
                    {
                        this._precompiledHeaderFile = value;
                        this.OnPropertyChanged("PrecompiledHeaderFile");
                    }
                }
                else
                {
                    this._precompiledHeaderFile = value;
                    this.OnPropertyChanged("PrecompiledHeaderFile");
                }
            }
        }

        [XmlIgnore()]
        public bool PrecompiledHeaderFileSpecified
        {
            get
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                return _precompiledHeaderFileSpecified;
            }
            set
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                _precompiledHeaderFileSpecified = value;
            }
        }

        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string PrecompiledHeaderThrough
        {
            get
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");
                if (this._precompiledHeaderThrough == null)
                    this._precompiledHeaderThrough = string.Empty;
                return this._precompiledHeaderThrough;
            }
            set
            {
                if ((this._precompiledHeaderThrough != null))
                {
                    Debug.Assert(this.nameField == "VCCLCompilerTool");

                    if ((_precompiledHeaderThrough.Equals(value) != true))
                    {
                        this._precompiledHeaderThrough = value;
                        this.OnPropertyChanged("PrecompiledHeaderThrough");
                    }
                }
                else
                {
                    this._precompiledHeaderThrough = value;
                    this.OnPropertyChanged("PrecompiledHeaderThrough");
                }
            }
        }

        [XmlIgnore()]
        public bool PrecompiledHeaderThroughSpecified
        {
            get
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                return _precompiledHeaderThroughSpecified;
            }
            set
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                _precompiledHeaderThroughSpecified = value;
            }
        }


        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string PreprocessorDefinitions
        {
            get
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");
                if (this._preprocessorDefinitions == null)
                    this._preprocessorDefinitions = string.Empty;
                return this._preprocessorDefinitions;
            }
            set
            {
                if ((this._preprocessorDefinitions != null))
                {
                    Debug.Assert(this.nameField == "VCCLCompilerTool");

                    if ((_preprocessorDefinitions.Equals(value) != true))
                    {
                        this._preprocessorDefinitions = value;
                        this._preprocessorDefinitionsSpecified = string.IsNullOrEmpty(_preprocessorDefinitions) == false;

                        this.OnPropertyChanged("PreprocessorDefinitions");
                    }
                }
                else
                {
                    this._preprocessorDefinitions = value;
                    this._preprocessorDefinitionsSpecified = string.IsNullOrEmpty(_preprocessorDefinitions) == false;

                    this.OnPropertyChanged("PreprocessorDefinitions");
                }
            }
        }

        [XmlIgnore()]
        public bool PreprocessorDefinitionsSpecified
        {
            get
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                return _preprocessorDefinitionsSpecified;
            }
            set
            {
                // Debug.Assert(this.nameField == "VCCLCompilerTool");
                _preprocessorDefinitionsSpecified = value;
            }
        }

        public string[] GetPreprocessorDefinitions()
        {
            return _preprocessorDefinitions.Split(new char[] {';'}, StringSplitOptions.RemoveEmptyEntries);
        }

        public void AddPreprocessorDefinition(string definition)
        {
            Debug.Assert(string.IsNullOrEmpty(definition) == false);

            if (_preprocessorDefinitions.Length != 0)
                PreprocessorDefinitions = PreprocessorDefinitions + ";";
            PreprocessorDefinitions = PreprocessorDefinitions + definition;
        }

        public void AddPreprocessorDefinitions(IEnumerable<string> definitions)
        {
            foreach (var definition in definitions)
            {
                AddPreprocessorDefinition(definition);
            }
        }

        public static PrecompiledHeaderOptions CreatePrecompiledHeaderOptions(ConfigurationTypeTool tool)
        {
            Debug.Assert(tool.Name == "VCCLCompilerTool");
            Debug.Assert(tool.NameSpecified == true);
            Debug.Assert(tool.AnyAttr != null);

            if (tool.UsePrecompiledHeaderSpecified == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            PrecompiledHeaderOptions options = new PrecompiledHeaderOptions();
            options.UsePrecompiledHeader =
                (UsePrecompiledHeaderOptions)
                Enum.Parse(typeof(UsePrecompiledHeaderOptions), tool.UsePrecompiledHeader);
            options.PrecompiledHeaderThrough = tool.PrecompiledHeaderThrough;
            options.PrecompiledHeaderFile = tool.PrecompiledHeaderFile;

            return options;
        }
        /*
        public static PrecompiledHeaderOptions CreateInstance(ConfigurationTypeTool tool)
        {
            Debug.Assert(tool.Name == "VCCLCompilerTool");
            Debug.Assert(tool.NameSpecified == true);
            Debug.Assert(tool.AnyAttr != null);

            if (tool.UsePrecompiledHeaderSpecified == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            PrecompiledHeaderOptions options = new PrecompiledHeaderOptions();
            options.UsePrecompiledHeader =
                (UsePrecompiledHeaderOptions)
                Enum.Parse(typeof(UsePrecompiledHeaderOptions), tool.UsePrecompiledHeader);
            options.PrecompiledHeaderThrough = tool.PrecompiledHeaderThrough;
            options.PrecompiledHeaderFile = tool.PrecompiledHeaderFile;

            return options;
        }
        */
    }
}