using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Ionic.Zip;
using Ionic.Zlib;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    /// <summary>
    /// The interface for compression class.
    /// </summary>
    /// <remarks></remarks>
    public interface IPack
    {
        /// <summary>
        /// Gets or sets the source folder. 
        /// </summary>
        /// <value>The source folder to be compressed.</value>
        /// <remarks>Here are some of supported compression algorithms
        /// <list type="bullet">
        /// <item>
        /// <description>*.zip</description>
        /// </item>
        /// <item>
        /// <description>*.7z</description>
        /// </item>
        /// </list>
        /// </remarks>
        [Required]        
        ITaskItem SrcFolder { get; set; }

        /// <summary>
        /// Gets or sets the excludes.
        /// </summary>
        /// <value>Pattern strings which refers to files to be excluded.</value>
        /// <remarks><example>*.txt; *.log</example></remarks>
        string[] Excludes { get; set; }

        /// <summary>
        /// Gets or sets the zip file path.
        /// </summary>
        /// <value>The destination file path.</value>
        /// <remarks></remarks>
        [Required]
        ITaskItem ZipFilePath { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether this <see cref="IPack"/> is overwrite.
        /// </summary>
        /// <value><c>true</c> if overwrite; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        bool Overwrite { get; set; }

        /// <summary>
        /// Gets or sets the zip level.
        /// </summary>
        /// <value>Compression rate. (0, 1, ... ,8 ,9).</value>
        /// <remarks>Higher number, higher compression rate.</remarks>
        int ZipLevel { get; set; }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        string Password { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether [contains root dir].
        /// </summary>
        /// <value><c>true</c> if [contains root dir]; otherwise, <c>false</c>.</value>
        /// <remarks></remarks>
        bool ContainsRootDir { get; set; }

        /// <summary>
        /// Executes this instance.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        bool Execute();
    }
}
