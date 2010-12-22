using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace UnityBuild
{
    public partial class ConfigurationTypeTool
    {
        private string _usePrecompiledHeaderField;
        private bool _usePrecompiledHeaderSpecified;

        private string _precompiledHeaderFile;
        private bool _precompiledHeaderFileSpecified;

        private string _precompiledHeaderThrough;
        private bool _precompiledHeaderThroughSpecified;
        
        [System.Xml.Serialization.XmlAttributeAttribute()]
        public string UsePrecompiledHeader
        {
            get
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");
                if (this._usePrecompiledHeaderField == null)
                    this._usePrecompiledHeaderField = string.Empty;
                return this._usePrecompiledHeaderField;
            }
            set
            {
                Debug.Assert(this.nameField == "VCCLCompilerTool");

                if ((this._usePrecompiledHeaderField != null))
                {
                    if ((_usePrecompiledHeaderField.Equals(value) != true))
                    {
                        this._usePrecompiledHeaderField = value;
                        this.OnPropertyChanged("UsePrecompiledHeader");
                    }
                }
                else
                {
                    this._usePrecompiledHeaderField = value;
                    this.OnPropertyChanged("UsePrecompiledHeader");
                }
            }
        }



        [XmlIgnore()]
        public bool UsePrecompiledHeaderSpecified
        {
            get
            {
                return _usePrecompiledHeaderSpecified;
            }
            set
            {
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
                Debug.Assert(this.nameField == "VCCLCompilerTool");

                if ((this._precompiledHeaderFile != null))
                {
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
                return _precompiledHeaderFileSpecified;
            }
            set
            {
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
                Debug.Assert(this.nameField == "VCCLCompilerTool");

                if ((this._precompiledHeaderThrough != null))
                {
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
                return _precompiledHeaderThroughSpecified;
            }
            set
            {
                _precompiledHeaderThroughSpecified = value;
            }
        }
    }
}
