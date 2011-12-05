using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    /// <summary>
    /// The interface for decompression class.
    /// </summary>
    /// <remarks></remarks>
    interface IUnpack
    {
        /// <summary>
        /// [Required] Gets or sets the target dir.
        /// </summary>
        /// <value>The folder where <see cref="ZipFilePath"/> will be decompressed into.</value>
        /// <remarks></remarks>
        [Required]
        ITaskItem TargetDir { get; set; }

        /// <summary>
        /// Gets or sets a value indicating whether this <see cref="IUnpack"/> is overwrite.
        /// </summary>
        /// <value><c>true</c> if overwrite; otherwise, <c>false</c>. If true, overwrite an old file/folder with new one if the same file/folder exists.</value>
        /// <remarks></remarks>
        bool Overwrite { get; set; }

        /// <summary>
        /// [Required] Gets or sets the zip file path.
        /// </summary>
        /// <value>The zip file path.</value>
        /// <remarks></remarks>
        [Required]
        ITaskItem ZipFilePath { get; set; }

        /// <summary>
        /// Gets or sets the password.
        /// </summary>
        /// <value>The password.</value>
        /// <remarks></remarks>
        string Password { get; set; }

        /// <summary>
        /// Executes this instance.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
        bool Execute();
    }
}
