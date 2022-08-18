<process class="com.nokia.carbide.cpp.project.core.processes.NewProject"
 		bundle="com.nokia.carbide.cpp.project.core">
	<parameter name="project" projectName="$(projectName)" /> 
</process>

<process class="com.nokia.carbide.cpp.project.core.processes.CreateRezId"
 		bundle="com.nokia.carbide.cpp.project.core">
	<parameter name="project" projectName="$(projectName)" /> 
</process>

<process class="com.nokia.carbide.templatewizard.processes.CreateFolders"
	 	bundle="com.nokia.carbide.templatewizard">
	<parameter name="project" projectName="$(projectName)" />
	<parameter name="folder" path="$(sourceDir)" /> 
	<parameter name="folder" path="$(incDir)" /> 
	<parameter name="folder" path="$(dataDir)" /> 
	<parameter name="folder" path="$(groupDir)" /> 
	<parameter name="folder" path="$(sisDir)" /> 
</process>

