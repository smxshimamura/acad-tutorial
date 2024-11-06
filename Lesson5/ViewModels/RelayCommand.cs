// --------------------------------------------------------------------------------------------------------------------
// <copyright file="RelayCommand.cs" company="Tutorial">
//   Tutorial codes.
// </copyright>
// <summary>
//   Defines the Commands type.
// </summary>
// --------------------------------------------------------------------------------------------------------------------

namespace Lesson5.ViewModels
{
    using System.Windows.Input;

    /// <summary>
    ///  The relay command.
    /// </summary>
    /// <seealso cref="System.Windows.Input.ICommand" />
    internal class RelayCommand : ICommand
    {
        /// <summary>
        /// The action.
        /// </summary>
        private readonly Action action;

        /// <summary>
        /// Initializes a new instance of the <see cref="RelayCommand"/> class.
        /// </summary>
        /// <param name="action">The action.</param>
        public RelayCommand(Action action)
        {
            this.action = action;
        }


        /// <summary>
        /// Occurs when changes occur that affect whether the command should execute.
        /// </summary>
        public event EventHandler? CanExecuteChanged;

        /// <summary>
        /// Defines the method that determines whether the command can execute in its current state.
        /// </summary>
        /// <param name="parameter">Data used by the command.  If the command does not require data to be passed, this object can be set to <see langword="null" />.</param>
        /// <returns>
        ///   <see langword="true" /> if this command can be executed; otherwise, <see langword="false" />.
        /// </returns>
        public bool CanExecute(object? parameter) => true;

        /// <summary>
        /// Defines the method to be called when the command is invoked.
        /// </summary>
        /// <param name="parameter">Data used by the command.  If the command does not require data to be passed, this object can be set to <see langword="null" />.</param>
        public void Execute(object? parameter)
        {
            this.action?.Invoke();
        }

    }
}
